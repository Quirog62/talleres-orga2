/* ** por compatibilidad se omiten tildes **
================================================================================
 TALLER System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de funciones del scheduler.
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "types.h"

void sched_init();
uint16_t sched_next_task();


void sched_add_task(uint16_t selector, int8_t task_id);
uint8_t sched_current_task();
void sched_enable_task(int8_t task_id);
void sched_disable_task(int8_t task_id);
extern int8_t current_task;
#endif //  __SCHED_H__
