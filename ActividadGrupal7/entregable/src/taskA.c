#include "syscall.h"
#include "stdbool.h"
#include "colors.h"
#include "i386.h"

#define KEY_SIZE 5
#define LEFT_MARGIN 0
#define TOP_MARGIN 15


void task(void) {
  //Clave a escribir en la pagina de memoria compartida
  char shared_key[] = {38,92,93,92,39,0};
  //Pide el id de la tarea actual
  int8_t task_id = syscall_getid();
  //Modifica la clave en base al id de la tarea
  for(int8_t k = 0; k < KEY_SIZE; k++){
	  shared_key[k] += task_id;
  }
  //Revisa que el id se encuentre dentro del rango de la paginas compartidas
  //porque va a usarlo para elegir la pagina en la cual escribir
  if(task_id < 0 || task_id > 31){
	  if(task_id == -1)
		  syscall_print("TASK_ID is from idle task", 10, 30, C_BG_BLACK | C_FG_LIGHT_BLUE);
	  else
		  syscall_print("TASK_ID out of range", 10, 30, C_BG_BLACK | C_FG_LIGHT_BLUE);
	  while (true) {
		  syscall_print_dec(task_id, LEFT_MARGIN, 30, C_BG_BLACK | C_FG_LIGHT_RED);
	    __asm volatile("nop");
	  }
  }
  //Pide una memorida compartida cuyo id sea igual al de la tarea
  vaddr_t shared_page_addr = syscall_getshm(task_id);
  //Castea la direccion como arreglo de bytes para escribir la clave
  char *current_byte = (char*)shared_page_addr;
  for(int8_t k = 0; k < KEY_SIZE; k++){
	  current_byte[0x10 * task_id + k] = shared_key[k];
  }
  //Libera la pagina de memoria compartida
  syscall_freeshm(task_id);
  //Escribe el estado al finalizar la ejecucion
  syscall_print_dec(task_id, LEFT_MARGIN, TOP_MARGIN + task_id, C_BG_BLACK | C_FG_LIGHT_BROWN);
  syscall_print("ESCRIBE ", LEFT_MARGIN + 3, TOP_MARGIN + task_id, C_BG_BLACK | C_FG_LIGHT_GREY);
  syscall_print(shared_key, LEFT_MARGIN + 11, TOP_MARGIN + task_id, C_BG_BLACK | C_FG_LIGHT_RED);
  syscall_print(" EN PAGINA COMPARTIDA: ", LEFT_MARGIN + 16, TOP_MARGIN + task_id, C_BG_BLACK | C_FG_LIGHT_GREY);
  syscall_print_dec(task_id, LEFT_MARGIN + 39, TOP_MARGIN + task_id, C_BG_BLACK | C_FG_LIGHT_RED);
  syscall_print(" EN DIR :", LEFT_MARGIN + 40, TOP_MARGIN + task_id, C_BG_BLACK | C_FG_LIGHT_GREY);
  syscall_print_hex((uint32_t)current_byte + 0x10 * task_id, LEFT_MARGIN + 49, TOP_MARGIN + task_id, C_BG_BLACK | C_FG_LIGHT_RED);

  while (true) {
	  __asm volatile("nop");
  }
}
