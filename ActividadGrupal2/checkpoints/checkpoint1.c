#include "checkpoints.h"

/** AUX FUNCTIONS **/

/* Funciones */

#define FLOAT_ARR_SIZE 10

/**
	Checkpoint 1: inicialización estática y dinámica
	Por favor completen la parte del código marcada con ? según lo indicado en el enunciado
*/

void checkpoint_1(){
	uint32_t i;	
	printf("[Checkpoint 1]:\n========\n");
	//Inicialice estáticamente un arreglo de floats de tamaño FLOAT_ARR_SIZE
	//COMPLETAR:
	float float_static_array [FLOAT_ARR_SIZE];


	printf("Inicialización estática:\n========\n");
	printf("\tArray de floats:\n");
	for(i = 0; i < FLOAT_ARR_SIZE; i++){

		//COMPLETAR:
		float_static_array[i]	= i / 2.0f;
		printf("\t[%i]:\t%f\n", i, float_static_array[i]);
	}
	
	//Inicialice dinámicamente un arreglo de floats de tamaño FLOAT_ARR_SIZE
	//COMPLETAR:
	float* float_dynamic_array = malloc(FLOAT_ARR_SIZE * sizeof(float));

	//Imprima los valores del arreglo por la salida estándar
	printf("Inicialización dinámica:\n========\n");
	printf("\tArray de floats:\n");
	for(i = 0; i < FLOAT_ARR_SIZE; i++){
		float_dynamic_array[i]	= i / 2.0f;
	//	COMPLETAR:	
		printf("\t[%i]:\t%f\n", i, float_dynamic_array[i]);
	}
		
	//Libere la región de memoria reservada para el arreglo dinámico
	//COMPLETAR:
	free(float_dynamic_array);
}
