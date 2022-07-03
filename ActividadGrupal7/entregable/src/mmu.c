/* ** por compatibilidad se omiten tildes **
================================================================================
 TALLER System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"

#include "kassert.h"

paddr_t shared_memory_paddr[SHARED_MEMORY_SIZE];

static pd_entry_t* kpd = (pd_entry_t*)KERNEL_PAGE_DIR;
static pt_entry_t* kpt = (pt_entry_t*)KERNEL_PAGE_TABLE_0;

static const uint32_t identity_mapping_end = 0x003FFFFF;
static const uint32_t user_memory_pool_end = 0x02FFFFFF;

static paddr_t next_free_kernel_page = 0x100000;
static paddr_t next_free_user_page = 0x400000;

/**
 * La inicializacion define las entradas de la tabla de memoria compartida en cero
 * (valor no inicializado)
 */
void mmu_init(void){
	for(size_t i = 0; i < SHARED_MEMORY_SIZE; i++){
		shared_memory_paddr[i] = 0;
	}
}


#define DST_VIRT_PAGE 0xA00000
#define SRC_VIRT_PAGE 0xB00000

static inline void copy_page(paddr_t dst_addr, paddr_t src_addr) {
  uint32_t cr3 = rcr3();
  mmu_map_page(cr3, SRC_VIRT_PAGE, src_addr, MMU_P);
  mmu_map_page(cr3, DST_VIRT_PAGE, dst_addr, MMU_P | MMU_W);

  uint8_t* src = (uint8_t*)SRC_VIRT_PAGE;
  uint8_t* dst = (uint8_t*)DST_VIRT_PAGE;

  for (size_t i = 0; i < PAGE_SIZE; i++) {
    dst[i] = src[i];
  }

  mmu_unmap_page(cr3, DST_VIRT_PAGE);
  mmu_unmap_page(cr3, SRC_VIRT_PAGE);
}

#define DST_SHARED_PAGE(mem_id)(TASK_CODE_VIRTUAL + PAGE_SIZE * (3 + (mem_id)))
/**
 * Devuelve la direccion virtual asociada a la pagina de memoria compartida
 * identificada con el valor mem_id
 */
vaddr_t mmu_getshm(uint8_t mem_id){
	kassert((mem_id < SHARED_MEMORY_SIZE), "(getshm)SHARED MEM ID OUT OF RANGE");
	if(shared_memory_paddr[mem_id] == 0){
		shared_memory_paddr[mem_id] = mmu_next_free_user_page();
	}

	vaddr_t vaddr = DST_SHARED_PAGE(mem_id);

	uint32_t cr3 = rcr3();
	mmu_map_page(cr3, vaddr, shared_memory_paddr[mem_id], MMU_P | MMU_U | MMU_W);
	return vaddr;
}
/**
 * Devuelve la entrada de la tabla de paginas asociada a la pagina de memoria compartida
 * identificada con el valor mem_id
 */
void mmu_freeshm(uint8_t mem_id){
	kassert((mem_id < SHARED_MEMORY_SIZE), "(freeshm)SHARED MEM ID OUT OF RANGE");

	vaddr_t vaddr = DST_SHARED_PAGE(mem_id);

	uint32_t cr3 = rcr3();
	mmu_unmap_page(cr3, vaddr);
}


/**
 * kmemset asigna a un rango de memoria interpretado como un
 * rango de bytes de largo n que comienza en s el valor c
 * @param s es el puntero al comienzo del rango de memoria
 * @param c es el valor a asignar en cada byte de s[0..n-1]
 * @param n es el tamaño en bytes a asignar
 * @return devuelve el puntero al rango modificado (alias de s)
*/
static inline void* kmemset(void* s, int c, size_t n) {
  uint8_t* dst = (uint8_t*)s;
  for (size_t i = 0; i < n; i++) {
    dst[i] = c;
  }
  return dst;
}



/**
 * zero_page limpia el contenido de una página que comienza en addr
 * @param addr es la dirección del comienzo de la página a limpiar
*/
static inline void zero_page(paddr_t addr) {
  kmemset((void*)addr, 0x00, PAGE_SIZE);
}

/**
 * mmu_next_free_kernel_page devuelve la dirección de la próxima página de kernel disponible
 * @return devuelve la dirección de memoria de comienzo de la próxima página libre de kernel
 */
paddr_t mmu_next_free_kernel_page(void) {
  kassert(next_free_kernel_page < identity_mapping_end, "OUT OF MEMORY");
  uint32_t res = next_free_kernel_page;
  next_free_kernel_page += PAGE_SIZE;

  return res;
}

/**
 * mmu_next_free_user_page devuelve la dirección de la próxima página de usuarix disponible
 * @return devuelve la dirección de memoria de comienzo de la próxima página libre de usuarix
 */
paddr_t mmu_next_free_user_page(void) {
  kassert(next_free_user_page < user_memory_pool_end, "OUT OF USER MEMORY");
  uint32_t res = next_free_user_page;
  next_free_user_page += PAGE_SIZE;

  return res;
  return 0;
}

/**
 * mmu_init_kernel_dir inicializa las estructuras de paginación vinculadas al kernel
 * @return devuelve la dirección de memoria de la página donde se encuentra el directorio de páginas usado por el kernel
 */
paddr_t mmu_init_kernel_dir(void) {

  // Inicializar un page table y page directory con 0.
  zero_page(KERNEL_PAGE_DIR);
  zero_page(KERNEL_PAGE_TABLE_0);

  kassert(VIRT_PAGE_DIR(identity_mapping_end) == 0,
          "Rango de Identity Mapping invalido.");

  kpd[0] = (pd_entry_t){
      .attrs = MMU_P | MMU_W,
      .pt = ((uint32_t)kpt) >> 12,
  };

  for (size_t i = 0; i <= VIRT_PAGE_TABLE(identity_mapping_end); i++) {
    kpt[i] = (pt_entry_t){
        .attrs = MMU_P | MMU_W,
        .page = i,
    };
  }
  return KERNEL_PAGE_DIR;
}

/**
 * mmu_map_page agrega las entradas necesarias a las estructuras de paginación de modo de que
 * la dirección virtual virt se traduzca en la dirección física phy con los atributos definidos en attrs
 * @param cr3 el contenido que se ha de cargar en un registro CR3 al realizar la traducción
 * @param virt la dirección virtual que se ha de traducir en phy
 * @param phy la dirección física que debe ser accedida (dirección de destino)
 * @param attrs los atributos a asignar en la entrada de la tabla de páginas
 */
void mmu_map_page(uint32_t cr3, vaddr_t virt, paddr_t phy, uint32_t attrs) {
  pd_entry_t* pd = (pd_entry_t*)CR3_TO_PAGE_DIR(cr3);

  int pdi = VIRT_PAGE_DIR(virt);
  if ((pd[pdi].attrs & MMU_P) == 0) {
    // No existe entrada en el directorio de tablas de paginas.
    // Debemos crear un page table y mapearlo.
    paddr_t new_pt = mmu_next_free_kernel_page();
    zero_page(new_pt);
    pd[pdi].pt = new_pt >> 12;
    pd[pdi].attrs |= MMU_P;
  }
  pd[pdi].attrs |= attrs;

  pt_entry_t* pt = (pt_entry_t*)MMU_ENTRY_PADDR(pd[pdi].pt);
  int pti = VIRT_PAGE_TABLE(virt);
  pt[pti] = (pt_entry_t){
      .attrs = attrs | MMU_P,
      .page = phy >> 12,
  };
  tlbflush();
  
}

/**
 * mmu_unmap_page elimina la entrada vinculada a la dirección virt en la tabla de páginas correspondiente
 * @param virt la dirección virtual que se ha de desvincular
 */
paddr_t mmu_unmap_page(uint32_t cr3, vaddr_t virt) {
  pd_entry_t* pd = (pd_entry_t*)CR3_TO_PAGE_DIR(cr3);

  int pdi = VIRT_PAGE_DIR(virt);
  if (!(pd[pdi].attrs | MMU_P))
    return 0;

  pt_entry_t* pt = (pt_entry_t*)MMU_ENTRY_PADDR(pd[pdi].pt);
  int pti = VIRT_PAGE_TABLE(virt);

  if (!(pt[pti].attrs | MMU_P))
    return 0;

  paddr_t unmapped = MMU_ENTRY_PADDR(pt[pti].page);

  pt[pti] = (pt_entry_t){
      .page = 0,
      .attrs = 0,
  };

  tlbflush();
  return unmapped;
}

 /**
 * mmu_init_task_dir inicializa las estructuras de paginación vinculadas a una tarea cuyo código se encuentra en la dirección phy_start
 * @pararm phy_start es la dirección donde comienzan las dos páginas de código de la tarea asociada a esta llamada
 * @return el contenido que se ha de cargar en un registro CR3 para la tarea asociada a esta llamada
 */
paddr_t mmu_init_task_dir(paddr_t phy_start) {
  const paddr_t cr3 = mmu_next_free_kernel_page();
  zero_page(cr3);

  const paddr_t stack = mmu_next_free_user_page();
  // No podemos poner en 0 el stack porque esta pagina fisica
  // no es parte de la memoria del kernel.
  // El CR3 actual tampoco tiene mapeada esa pagina.

  for (vaddr_t i = 0; i < identity_mapping_end; i += PAGE_SIZE) {
    mmu_map_page(cr3, i, i, MMU_P | MMU_W);
  }



  mmu_map_page(cr3, TASK_CODE_VIRTUAL + PAGE_SIZE * 0, phy_start,
               MMU_P | MMU_U);
  mmu_map_page(cr3, TASK_CODE_VIRTUAL + PAGE_SIZE * 1, phy_start + PAGE_SIZE,
               MMU_P | MMU_U);

  mmu_map_page(cr3, TASK_CODE_VIRTUAL + PAGE_SIZE * 2, stack,
               MMU_P | MMU_U | MMU_W);

  return cr3;
}


