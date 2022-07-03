#ifndef CHECKPOINTS_H
#define CHECKPOINTS_H
/*
Sobre Ifndef
======
ifndef es una construcción de pre compilación que 
sólo incorpora el texto entre "#ifndef SYMBOL ... #endif 
sólo si el símbolo SYMBOL aún no fue definido
se suele utilizar esta combinación de 
#ifndef SYMBOL
#define SYMBOL
para que el pre-procesador sólo evalúe una vez los
encabezados antes de comenzar la compilación

Sobre includes
==============
Recuerden que los include son directivas que incluyen,
en este caso, las definiciones y encabezados de las funciones de uso común que queremos 
utilizar en nuestro programa
*/
#include <stdio.h> 	//encabezado de funciones de entrada y salida fopen, fclose, fgetc, printf, fprintf ...
#include <stdlib.h>	//biblioteca estándar, atoi, atof, rand, srand, abort, exit, system, NULL, malloc, calloc, realloc...
#include <stdint.h>	//contiene la definición de tipos enteros ligados a tamaños int8_t, int16_t, uint8_t,...
#include <ctype.h>	//contiene funciones relacionadas a caracteres, isdigit, islower, tolower...
#include <string.h>	//contiene las funciones relacionadas a strings, memcmp, strcat, memset, memmove, strlen,strstr...
#include <math.h>	//define funciones matemáticas como cos, sin, abs, sqrt, log...
#include <stdbool.h>	//contiene las definiciones de datos booleanos, true (1), false (0)
#include <unistd.h>	//define constantes y tipos standard, NULL, R_OK, F_OK, STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO...
#include <assert.h>	//provee la macro assert que evalúa una condición, y si no se cumple provee información diagnóstica y aborta la ejecución

//*************************************
//Declaración de constantes (directivas de pre-compilación)
//*************************************

#define SIMPLE_ITEM_LIST_INITIAL_SIZE	2
#define ARR_LENGTH	4
//*************************************
//Declaración de estructuras
//*************************************
typedef struct simple_item_str{
    uint32_t x;
    uint64_t y;
    uint32_t z;
} simple_item;

typedef struct  __attribute__((__packed__)) packed_simple_item_str{
    uint32_t x;
    uint64_t y;
    uint32_t z;
} packed_simple_item; 


typedef struct complex_item_str{
    uint64_t w;    
    uint32_t x;
    uint64_t y;
    uint32_t z;
} complex_item;

typedef struct  __attribute__((__packed__)) packed_complex_item_str{
    uint64_t w;    
    uint32_t x;
    uint64_t y;
    uint32_t z;
} packed_complex_item;

typedef struct simple_item_list_str{
	uint32_t size;
	uint32_t count;
	simple_item *list_array;
} simple_item_list;
//*************************************
//Declaración de funciones de ejemplo
//*************************************
extern simple_item *create_simple_item_array(uint32_t length);
extern simple_item *create_simple_item_array_initialized(uint32_t length, uint32_t default_x
	, uint64_t default_y, uint32_t default_z);

extern packed_simple_item *create_packed_simple_item_array(uint32_t length);
extern packed_simple_item *create_packed_simple_item_array_initialized(uint32_t length, uint32_t default_x
	, uint64_t default_y, uint32_t default_z);
	
simple_item_list *create_simple_item_list();	
void destroy_simple_item_list(simple_item_list *item_list);
extern void add_item_simple_item_list(simple_item_list *list, uint32_t x, uint64_t y, uint32_t z);	

//a explicar
uint32_t alternate_sum_4(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4);
uint32_t alternate_sum_4_simplified(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4);
uint32_t alternate_sum_8(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4, uint32_t x5, uint32_t x6, uint32_t x7, uint32_t x8);
uint32_t sum_z(simple_item *arr, uint32_t arr_length);
uint32_t packed_sum_z(packed_simple_item *arr, uint32_t arr_length);
void product_2(uint32_t * destination, uint32_t x1, uint32_t x2);
void product_2_f(uint32_t * destination, uint32_t x1, float f1);
//TAREA

uint32_t complex_sum_z(complex_item *arr, uint32_t arr_length);
uint32_t packed_complex_sum_z(packed_complex_item *arr, uint32_t arr_length);
void product_9_f(double * destination
, uint32_t x1, float f1, uint32_t x2, float f2, uint32_t x3, float f3, uint32_t x4, float f4
, uint32_t x5, float f5, uint32_t x6, float f6, uint32_t x7, float f7, uint32_t x8, float f8
, uint32_t x9, float f9);

//*************************************
//Declaración de funciones de ejemplo
//*************************************
void checkpoint_2();
void checkpoint_3();
void extras();



#endif
