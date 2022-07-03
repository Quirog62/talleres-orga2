#include "checkpoints.h"

/** AUX FUNCTIONS **/

/* Funciones */


/**
	Checkpoint 2: Explicar las funciones que figuran en el enunciado.
	- alternate_sum_4
	- alternate_sum_8
	- sum_z
	- packed_sum_z
	- product_2
	- product_2_f
*/

void checkpoint_2(){
	printf("Ejecutando el Checkpoint 2:\n==================\n");
	
	//Ejemplos básicos de interacción
	printf("Ejemplos básicos de interacción.\n");	
	uint32_t x1 = 0, x2 = 0, x3 = 0, x4 = 0, x5 = 0, x6 = 0, x7 = 0, x8 = 0, result = 0;
	x1 = x2 = x3 = x4 = 2;
	result = alternate_sum_4(x1,x2,x3,x4);
	printf("\t alternate_sum_4(%d,%d,%d,%d)=%d\n", x1, x2, x3, x4, result);
	result = alternate_sum_4_simplified(x1,x2,x3,x4);	
	printf("\t alternate_sum_4_simplified(%d,%d,%d,%d)=%d\n", x1, x2, x3, x4, result);
	x1 = 8, x2 = 6, x3 = 4, x4 = 2;
	printf("\t alternate_sum_4(%d,%d,%d,%d)=%d\n", x1, x2, x3, x4, alternate_sum_4(x1,x2,x3,x4));
	printf("\t alternate_sum_4_simplified(%d,%d,%d,%d)=%d\n", x1, x2, x3, x4, alternate_sum_4(x1,x2,x3,x4));			
	x1 = x2 = x3 = x4 = x5 = x6 = x7 = x8 = 2;
	
	//Uso de parámetros en registros y la pila
	printf("Uso de parámetros en registros y la pila.\n");		
	printf("\t alternate_sum_8(%d,%d,%d,%d,%d,%d,%d,%d)=%d\n", x1, x2, x3, x4, 
		x5, x6, x7, x8, alternate_sum_8(x1,x2,x3,x4,x5,x6,x7,x8));	
	x1 = 16, x2 = 14, x3 = 12, x4 = 10, x5 = 8, x6 = 6, x7 = 4, x8 = 2;
	printf("\t alternate_sum_8(%d,%d,%d,%d,%d,%d,%d,%d)=%d\n", x1, x2, x3, x4, 
		x5, x6, x7, x8, alternate_sum_8(x1,x2,x3,x4,x5,x6,x7,x8));
	
	//Alineación de datos y cálculo de offsets y tamaños
	printf("Alineación de datos y cálculo de offsets y tamaños.\n");				
	simple_item arr[ARR_LENGTH];
	uint32_t i;
	for(i =  0; i < ARR_LENGTH; i++){ 
		arr[i].x = 0; 
		arr[i].y = 0; 
		arr[i].z = 2 * i; 
	}
	for(i = 1; i < ARR_LENGTH + 1; i++){
		result = sum_z(arr, i);
		printf("\tsum_z(arr,%d)=%d\n", i, result);
	}
	packed_simple_item packed_arr[ARR_LENGTH];
	for(i =  0; i < ARR_LENGTH; i++){ 
		packed_arr[i].x = 0; 
		packed_arr[i].y = 0; 
		packed_arr[i].z = 2 * i; 
	}
	for(i = 1; i < ARR_LENGTH + 1; i++){
		result = packed_sum_z(packed_arr, i);
		printf("\tpacked_sum_z(packed_arr,%d)=%d\n", i, result);
	}
	
	//Pasaje por referencia y conversion entre enteros y flotantes
	printf("Pasaje por referencia y conversion entre enteros y flotantes.\n");		
	uint32_t product_result = 0;
	uint32_t a = 10, b = 5;
	float c = 0.2;
	product_2(&product_result, a, b);
	printf("product_2(%p,%d,%d)=%d\n", (void*)&product_result, a, b, product_result);
	product_2_f(&product_result, a, c);
	printf("product_2_f(%p,%d,%f)=%d\n", (void*)&product_result, a, c, product_result);	
}
