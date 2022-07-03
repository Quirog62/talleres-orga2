/* ** por compatibilidad se omiten tildes **
================================================================================
 TALLER System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  Definicion de llamadas a sistema, para usar desde las tareas.
*/

#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include "stdbool.h"
#include "stdint.h"
#include "types.h"

#define LS_INLINE static __inline __attribute__((always_inline))
#define NORETURN  __attribute__((noreturn))

#define UNREACHABLE()                                                          \
  while (1) {                                                                  \
    __asm__ volatile("pause" ::: "memory");                                    \
  }

/*
 * Syscalls
 */


LS_INLINE vaddr_t syscall_getshm(uint8_t mem_id) {
	paddr_t ret;
  __asm__ volatile("int $88"        /* int. de soft. 88 */
                   : "=a"(ret)      /* devuelve el valor en eax ("a") */
                   : "a"(mem_id) /* pasa el valor mem_id en eax ("a")*/
                   : "memory",
                     "cc"); /* announce to the compiler that the memory and
                               condition codes have been modified */
  return ret;
}

LS_INLINE void syscall_freeshm(uint8_t mem_id) {
  __asm__ volatile("int $98" /* int. de soft. 98 */
                   :
                   : "a"(mem_id) /* pasa el valor mem_id en eax("a")*/
                   : "memory",
                     "cc"); /* announce to the compiler that the memory and
                               condition codes have been modified */
}

LS_INLINE int8_t syscall_getid(void) {
	int8_t ret;
  __asm__ volatile("int $108"        /* int. de soft. 108 */
                   : "=a"(ret)      /* devuelve el resultado en eax ("a") */
                   :
                   : "memory",
                     "cc"); /* announce to the compiler that the memory and
                               condition codes have been modified */
  return ret;
}

LS_INLINE void syscall_print(const char* text, uint32_t x, uint32_t y, uint16_t attr) {
  __asm__ volatile("int $118" /* int. de soft. 118 */
                   :
                   : "a"(text), "b"(x), "c"(y), "d"(attr)
                   : "memory",
                     "cc"); /* announce to the compiler that the memory and
                               condition codes have been modified */
}

LS_INLINE void syscall_print_dec(uint32_t numero, uint32_t x, uint32_t y,
        uint16_t attr){
  __asm__ volatile("int $128" /* int. de soft. 128 */
                   :
          	  	   : "a"(numero), "b"(x), "c"(y), "d"(attr)
                   : "memory",
                     "cc"); /* announce to the compiler that the memory and
                               condition codes have been modified */
}

LS_INLINE void syscall_print_hex(uint32_t numero, uint32_t x, uint32_t y,
        uint16_t attr){
  __asm__ volatile("int $138" /* int. de soft. 138 */
                   :
          	  	   : "a"(numero), "b"(x), "c"(y), "d"(attr)
                   : "memory",
                     "cc"); /* announce to the compiler that the memory and
                               condition codes have been modified */
}

#endif //  __SYSCALL_H__
