#include "syscall.h"
#include "stdbool.h"
#include "colors.h"

#define SHARED_MEMORY_SIZE 32
#define PAGE_SIZE 4096
#define KEY_SIZE 5

#define LEFT_MARGIN 0
#define TOP_MARGIN 15


void task(void) {
  //Clave a encontrar en la memoria compartida
  char shared_key[] = "(^_^)";
  bool key_found = false;
  int8_t key_page = 0;
  vaddr_t found_at = 0;
  //Pide id de la tarea actual
  int8_t task_id = syscall_getid();
  while (!key_found) {
    for(int8_t i = 0; (i < SHARED_MEMORY_SIZE) && !key_found; i++){
    	//Pide acceso a la pagina de memoria compartida cuya identidad sea i
    	vaddr_t shared_page_addr = syscall_getshm(i);
    	//Castea la direccion como arreglo de bytes para buscar la clave
    	char *current_byte = (char*)shared_page_addr;
    	for(int32_t j = 0; (j < (PAGE_SIZE - KEY_SIZE)) && !key_found; j++){
    		key_found = true;
    		for(int8_t k = 0; k < KEY_SIZE; k++){
    			if(current_byte[j + k] != shared_key[k]){
    				key_found = false;
    				break;
    			}
    		}
			found_at = shared_page_addr + j;
    	}
    	//Libera la pagina pedida
    	syscall_freeshm(i);
    	if(key_found){
    		key_page = i;
    	}
    }
    //Escribe el estado al finalizar la ejecucion
    syscall_print_dec(task_id, LEFT_MARGIN, TOP_MARGIN + task_id, C_BG_BLACK | C_FG_LIGHT_CYAN);
    syscall_print("BUSCA (^_^) SIN EXITO", LEFT_MARGIN + 3, TOP_MARGIN + task_id, C_BG_BLACK | C_FG_LIGHT_BLUE);
  }
  //Escribe el estado al finalizar la ejecucion
  syscall_print_dec(task_id, LEFT_MARGIN, TOP_MARGIN + task_id, C_BG_BLACK | C_FG_LIGHT_CYAN);
  syscall_print("ENCUENTRA (^_^) EN PAG. COMPARTIDA: ", LEFT_MARGIN + 3, TOP_MARGIN + task_id, C_BG_BLACK | C_FG_LIGHT_BLUE);
  syscall_print_dec(key_page, LEFT_MARGIN + 38, TOP_MARGIN + task_id, C_BG_BLACK | C_FG_LIGHT_RED);
  syscall_print(" DIR: ", LEFT_MARGIN + 39, TOP_MARGIN + task_id, C_BG_BLACK | C_FG_LIGHT_BLUE);
  syscall_print_hex(found_at, LEFT_MARGIN + 44, TOP_MARGIN + task_id, C_BG_BLACK | C_FG_LIGHT_RED);
  while (true) {
    __asm volatile("nop");
  }
}
