/* ** por compatibilidad se omiten tildes **
================================================================================
 TALLER System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "task.h"
#include "types.h"

#include "defines.h"
#include "i386.h"
#include "kassert.h"
#include "mmu.h"
#include "sched.h"
#include "screen.h"
#include "tss.h"

// Constantes de pantalla para tareas
#define OFFSET_TASK_ID   0
#define OFFSET_TIPO      8
#define OFFSET_ID       15
#define OFFSET_CLOCK    20
#define OFFSET_ROW       3
#define OFFSET_TICKS    50
#define OFFSET_TS       60

size_t tick_count = 0;
static size_t task_switchs = 0;
static int8_t last_task = -1;

// Enum para definir tipos de tarea
typedef enum tipo_e {
  TASK_A = 0,
  TASK_B = 1,
} tipo_e;

// Estructura que vincual tarea y tipo
typedef struct task_t {
  uint8_t task_id; 
  tipo_e tipo;
} task_t;

// Arreglo de tareas segun tipo
static task_t task[2][MAX_TASKS/2] = {0};

// Definición de templates para cada tipo de tarea
typedef struct info_tipo_t {
  paddr_t code_start;
  char* map_char;
  uint16_t map_color;
} info_tipo_t;

static info_tipo_t info_tipo[2] = {
    [TASK_A] =
        {
            .code_start = TASK_A_CODE_START,
            .map_char = "P",
            .map_color = C_FG_WHITE | C_BG_RED,
        },
    [TASK_B] =
        {
            .code_start = TASK_B_CODE_START,
            .map_char = "G",
            .map_color = C_FG_WHITE | C_BG_BLUE,

        },
};

static inline uint8_t tid2tipo(uint8_t task_id) {
  return task_id >= MAX_TASKS/2? TASK_B : TASK_A;
}

static inline uint8_t tid2idx(uint8_t task_id) 
{ return task_id >= MAX_TASKS/2? task_id - MAX_TASKS/2: task_id;}


char clock_states[] = "-\\|/";

typedef struct clock_info_t {
  uint8_t x;
  uint8_t y;
  uint8_t idx;
} clock_info_t;

clock_info_t relojes[] = {
    {.x = OFFSET_CLOCK, .y = OFFSET_ROW + 0, .idx = 0}, 
    {.x = OFFSET_CLOCK, .y = OFFSET_ROW + 1 , .idx = 0},
    {.x = OFFSET_CLOCK, .y = OFFSET_ROW + 2 , .idx = 0},
    {.x = OFFSET_CLOCK, .y = OFFSET_ROW + 3 , .idx = 0},
    {.x = OFFSET_CLOCK, .y = OFFSET_ROW + 4 , .idx = 0},
    {.x = OFFSET_CLOCK, .y = OFFSET_ROW + 5 , .idx = 0},
    {.x = OFFSET_CLOCK, .y = OFFSET_ROW + 6 , .idx = 0},
    {.x = OFFSET_CLOCK, .y = OFFSET_ROW + 7 , .idx = 0},
    {.x = OFFSET_CLOCK, .y = OFFSET_ROW + 8 , .idx = 0},
    {.x = OFFSET_CLOCK, .y = OFFSET_ROW + 9 , .idx = 0},
};

static inline void dibujar_reloj(uint8_t task_id) {
  if (task_id >= MAX_TASKS) {
    print_dec(task_id, 0, 0, C_BG_BLACK | C_FG_LIGHT_GREEN);
    while (true)
      __asm volatile("nop");
  }
  char c[2] = {0};
  uint8_t state_id = (relojes[task_id].idx + 1) % 4;
  relojes[task_id].idx = state_id;
  c[0] = clock_states[state_id];

  print(c, relojes[task_id].x, relojes[task_id].y, C_BG_BLACK | C_FG_CYAN);
}

/**
 * Crea una tarea en base a su id en la tabla de tss, tipo e indice de tabla en task
 */
static void create_task(uint8_t task_id, tipo_e tipo, uint8_t indice) {
  tss_tasks[task_id] = tss_create_user_task(info_tipo[tipo].code_start);

  task[tipo][indice] = (task_t){
      .task_id = task_id,
      .tipo = tipo,
  };

  size_t gdt_id = (tipo == TASK_A? GDT_IDX_TASK_A_START : GDT_IDX_TASK_B_START) + indice;
  gdt[gdt_id] = tss_gdt_entry_for_task(&tss_tasks[task_id]);
  sched_add_task(gdt_id << 3, task_id);
}

void disable_task(int8_t task_id) {
  sched_disable_task(task_id);

}

void print_title(void){
    print("TASK_ID",OFFSET_TASK_ID,0 ,C_FG_LIGHT_MAGENTA | C_BG_BLACK);
    print("TIPO"   ,OFFSET_TIPO   ,0 ,C_FG_LIGHT_MAGENTA | C_BG_BLACK);
    print("ID"     ,OFFSET_ID     ,0 ,C_FG_LIGHT_MAGENTA | C_BG_BLACK);
    print("CLOCK"  ,OFFSET_CLOCK  ,0 ,C_FG_LIGHT_MAGENTA | C_BG_BLACK);
    print("TICKS"  ,OFFSET_TICKS  ,0,C_FG_LIGHT_MAGENTA | C_BG_BLACK);
    print("TASK SWITCHS"  ,OFFSET_TS     ,0,C_FG_LIGHT_MAGENTA | C_BG_BLACK);

}

void print_task_row(int8_t task_id, uint8_t color){
    if (task_id == IDLE_TASK_ID) return;

    print_dec(task_id, OFFSET_TASK_ID,OFFSET_ROW + task_id, color);
    char * tipo = tid2tipo(task_id) == TASK_A? "TASK_A": "TASK_B";
    print(tipo,OFFSET_TIPO, OFFSET_ROW + task_id, color);
    print_dec(tid2idx(task_id),OFFSET_ID  , OFFSET_ROW + task_id, color);
}

void task_tick(void) {
  if (current_task != IDLE_TASK_ID) {
    dibujar_reloj(current_task);
   
    if (current_task != last_task){
      // task switch
      print_task_row(last_task, C_FG_LIGHT_RED | C_BG_BLACK);
      last_task = current_task;
      print_task_row(current_task, C_FG_LIGHT_GREEN | C_BG_BLACK);  
    }    
  }

  tick_count += 1;
  print_dec(tick_count,OFFSET_TICKS,1,C_FG_LIGHT_MAGENTA | C_BG_BLACK);
  if (tick_count % TICKS_TASK_SWITCH == 0){
      task_switchs++;
      print_dec(task_switchs,OFFSET_TS,1,C_FG_LIGHT_MAGENTA | C_BG_BLACK);
  }
}

void task_init(void) {

  // Se crean las tss de usuario y se agregan a la gdt y al scheduler. 

  // Se asume tareas TASK_A tienen task_id de 0 a MAX_TASKS/2 - 1: 0,1,2...  
  // Se asume tareas TASK_B tienen task_id de MAX_TASKS/2 a MAX_TASKS - 1: 5,6,7...  
  print_title();

   for (uint8_t i = 0; i < MAX_TASKS/2; i++) {
     create_task(i, TASK_A, i);
     sched_enable_task(i);
   }
  
   for (uint8_t i = 0; i < MAX_TASKS/2; i++) {
     create_task(MAX_TASKS/2 + i, TASK_B, i);
     sched_enable_task(MAX_TASKS/2 + i);
   }

}
