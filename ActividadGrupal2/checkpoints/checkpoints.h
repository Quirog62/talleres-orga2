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

#define DEFAULT_X_VALUE	0
#define DEFAULT_Y_VALUE	1
#define DEFAULT_Z_VALUE	1.0f

//*************************************
//Declaración de estructuras
//*************************************
typedef struct simple_item_str{
    uint32_t x;
    uint64_t y;
    float z;
} simple_item;

typedef struct composite_item_str{
    char *name;
    uint32_t value;
} composite_item;


//*************************************
//Declaración de funciones de ejemplo
//*************************************
void simple_item_static_initialization();
void simple_item_dynamic_initialization();

void sum_product_array(int32_t *arr, uint32_t len, int32_t *ptr_sum, int32_t *ptr_prod);
void set_string_at_location(char** destination, char* source);
void set_int_at_location(int32_t** destination, int32_t* source, uint32_t length);
//declaración de funciones de los checkpoints
void checkpoint_1();
void checkpoint_2();
#endif

