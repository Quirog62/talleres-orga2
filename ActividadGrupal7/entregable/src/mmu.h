/* ** por compatibilidad se omiten tildes **
================================================================================
 TALLER System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "types.h"

typedef struct pd_entry_t {
  uint32_t attrs : 12;
  uint32_t pt : 20;
} __attribute__((packed)) pd_entry_t;

typedef struct pt_entry_t {
  uint32_t attrs : 12;
  uint32_t page : 20;
} __attribute__((packed)) pt_entry_t;

#define SHARED_MEMORY_SIZE 32

extern paddr_t shared_memory_paddr[SHARED_MEMORY_SIZE];

void mmu_init(void);

vaddr_t mmu_getshm(uint8_t mem_id);

void mmu_freeshm(uint8_t mem_id);

paddr_t mmu_next_free_kernel_page(void);

paddr_t mmu_next_free_user_page(void);

void mmu_map_page(uint32_t cr3, vaddr_t virt, paddr_t phy, uint32_t attrs);

paddr_t mmu_unmap_page(uint32_t cr3, vaddr_t virt);

paddr_t mmu_init_kernel_dir(void);

paddr_t mmu_init_task_dir(paddr_t phy_start);
#endif //  __MMU_H__
