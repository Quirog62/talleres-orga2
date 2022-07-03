#include "checkpoints.h"

/** AUX FUNCTIONS **/

/* Funciones */


/**
	Checkpoint 3: Complete las funciones referidas en el enunciado, las funciones se encuentran definidas en
	checkpoint32.asm y son
	- complex_sum_z
	- packed_complex_sum_z
	- product_9
*/

void checkpoint_3(){
	uint32_t i, result;
	printf("Ejecutando el Checkpoint 3:\n==================\n");
	//Ejemplos básicos de interacción
	//Alineación de datos y cálculo de offsets y tamaños
	printf("Alineación de datos y cálculo de offsets y tamaños.\n");		
	complex_item complex_arr[ARR_LENGTH];
	for(i =  0; i < ARR_LENGTH; i++){ 
		complex_arr[i].w = 0; 
		complex_arr[i].x = 0; 
		complex_arr[i].y = 0; 
		complex_arr[i].z = 2 * i; 
	}
	for(i = 1; i < ARR_LENGTH + 1; i++){
		result = complex_sum_z(complex_arr, i);
		printf("\t complex_sum_z(complex_arr,%d)=%d\n", i, result);
	}
	packed_complex_item packed_complex_arr[ARR_LENGTH];
	for(i =  0; i < ARR_LENGTH; i++){ 
		packed_complex_arr[i].w = 0; 
		packed_complex_arr[i].x = 0; 
		packed_complex_arr[i].y = 0; 
		packed_complex_arr[i].z = 2 * i; 
	}
	for(i = 1; i < ARR_LENGTH + 1; i++){
		result = packed_complex_sum_z(packed_complex_arr, i);
		printf("\t packed_complex_sum_z(packed_complex_arr,%d)=%d\n", i, result);
	}	
	//Pasaje por referencia y conversion entre enteros y flotantes
	printf("Pasaje por referencia y conversion entre enteros y flotantes.\n");		
	double product_result = 0.0f;
	product_9_f(&product_result, 1, 2.0f, 1, 2.0f, 1, 2.0f, 1, 2.0f, 1, 2.0f, 1, 2.0f, 1, 2.0f
		, 1, 2.0f, 1, 2.0f);
	printf("\tproduct_9_f(%p,...)=%f\n", (void*)&product_result, product_result);
}
