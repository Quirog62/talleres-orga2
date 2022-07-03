#include "checkpoints.h"

/** AUX FUNCTIONS **/

/* Funciones */

/**
Esta función inicializa un arreglo estáticamente.
Noten que el arreglo puede utilizarse sólo dentro del contexto de la función. (scope, alcance)
Pregunta: ¿Qué sucede si intentan devolverlo por parámetro?
**/

#define ARR_LENGTH	3

//*************************************
//Implementación de funciones de ejemplo
//*************************************

/**
	Aquí vamos a ver algunos casos de inicialización estática.
*/
void simple_item_static_initialization(){
	//así declaramos el arreglo estáticamente, ¿qué valores tiene cada elemento?
	simple_item not_initialized_struct_array[ARR_LENGTH];
	//así declaramos el arreglo estáticamente, en la misma línea lo inicializamos también estáticamente
	//¿qué piensan que hace el compilador?
	simple_item initialized_struct_array[ARR_LENGTH] = {{2,3,1.0f},{1,5,123.0f},{0,0,0.0f}};	
	
	/*
	Recuerden que printf toma varios indicadores de tipo a imprimir
	%d	es un entero con signo
	%ld	es para enteros con signo de 64 bits
	%f	para flotantes
	%s	para strings
	%p	para punteros
	y que los siguientes caracteres tienen un significado especial
	\t	ingresa una tabulación
	\n	ingresa un quiebre de línea
	\0	es el caracter nulo que se usa para cerrar strings
	*/
	printf("Inicialización estática:\n================\n");
	uint32_t i;
	printf("Arrays de structs:\n=====\n");
	for(i = 0; i < ARR_LENGTH; i++)
		printf("\tElemento [%d]:{x:%d,\ty:%ld,\tz:%f}\n" 
			, i, initialized_struct_array[i].x, initialized_struct_array[i].y
			, initialized_struct_array[i].z);
	
	printf("Arrays de tipos nativos:\n=====\n");
	//no es necesario que los elementos sean estructuras, pueden ser ints o chars
	int32_t not_initialized_int_array[ARR_LENGTH];
	int32_t initialized_int_array[ARR_LENGTH]	= {1,2,3};	
	printf("\tArray de int: {%d, %d, %d}\n", initialized_int_array[0], initialized_int_array[1], initialized_int_array[2]);
	char not_initialized_char_array[ARR_LENGTH];
	char initialized_char_array[ARR_LENGTH]	= {'e','y','\0'};	
	char initialized_char_array_b[]		= "ey";		
	printf("\tArray de char (1): %s\n", initialized_char_array);
	printf("\tArray de char (2): %s\n", initialized_char_array_b);	
}

/**
	Aquí vamos a ver algunos casos de inicialización dinámica.
*/
void simple_item_dynamic_initialization(){
	//así declaramos el arreglo dinámicamente, ¿qué valores tiene cada elemento?
	//¿Dónde se ubican?
	//Recuerden que podemos utilizar punteros para referirnos a arreglos, la semántica del índice arr[i]
	//se resuelve de acuerdo al tipo declarado. Y sizeof es una directiva que devuelve el tamaño del dato en bytes
	//en tiempo de compilación. malloc retorna un puntero a la primera posición de memoria del espacio alocado
	simple_item *not_initialized_struct_array	= malloc(sizeof(simple_item) * ARR_LENGTH);

	//Así declaramos el arreglo dinámicamente inicializando sus valores en cero.
	//Calloc asigna todos los bytes de la región pedida a cero, sin importar su estructura
	//¿Qué forma de inicializar piensan que puede ser más rápida?
	simple_item *initialized_struct_array = calloc(ARR_LENGTH, sizeof(simple_item));	
	printf("Inicialización dinámica:\n================\n");	
	uint32_t i;
	printf("Arrays de structs:\n=====\n");
	for(i = 0; i < ARR_LENGTH; i++){
		printf("\tElemento [%d]:{x:%d,\ty:%ld,\tz:%f}\n" 
			, i, initialized_struct_array[i].x, initialized_struct_array[i].y
			, initialized_struct_array[i].z);
	}
	printf("Inicializando en valores por defecto:\n=====\n");
	for(i = 0; i < ARR_LENGTH; i++){
		initialized_struct_array[i].x	= DEFAULT_X_VALUE;
		initialized_struct_array[i].y	= DEFAULT_Y_VALUE;
		initialized_struct_array[i].z	= DEFAULT_Z_VALUE;
		printf("\tElemento [%d]:{x:%d,\ty:%ld,\tz:%f}\n" 
			, i, initialized_struct_array[i].x, initialized_struct_array[i].y
			, initialized_struct_array[i].z);
	}	
	
	//Siempre debemos liberar la memoria que pedimos al manejador de memoria dinámica
	free(not_initialized_struct_array);
	free(initialized_struct_array);	
}

