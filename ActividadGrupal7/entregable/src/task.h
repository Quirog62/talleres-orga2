/* ** por compatibilidad se omiten tildes **
================================================================================
 TALLER System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de las rutinas asociadas a las tareas
*/

#ifndef __TASK_H__
#define __TASK_H__
#include "types.h"

void task_tick(void);
void task_init(void);


void disable_task(int8_t task_id);

#endif //  __TASK_H__
