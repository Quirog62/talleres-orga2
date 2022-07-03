#include "checkpoints.h"

/** AUX FUNCTIONS **/
simple_item_list *create_simple_item_list(){
	simple_item_list *item_list	= malloc(sizeof(simple_item_list));
	item_list->size		= SIMPLE_ITEM_LIST_INITIAL_SIZE;
	item_list->count	= 0;	
	item_list->list_array	= create_simple_item_array(item_list->size);
	return item_list;
}

void destroy_simple_item_list(simple_item_list *item_list){
	item_list->size 	= 0;
	item_list->count	= 0;
	free(item_list->list_array);
	item_list->list_array	= NULL;
	free(item_list);
}


/* Funciones */
/**
	Extras: Ejercicios para leer de tarea
*/

void extras(){
	uint32_t i, j;
	
	simple_item *simple_array	= create_simple_item_array(ARR_LENGTH);
	for(i = 0; i < ARR_LENGTH; i++){
		printf("&simple_array = %p\n", (void*)simple_array);
	}
	packed_simple_item *packed_simple_array	= create_packed_simple_item_array(ARR_LENGTH);
	for(i = 0; i < ARR_LENGTH; i++){
		printf("&packed_simple_array = %p\n", (void*)packed_simple_array);
	}	
	free(simple_array);
	free(packed_simple_array);
	simple_array	= create_simple_item_array_initialized(ARR_LENGTH, 3, 4, 5);
	for(i = 0; i < ARR_LENGTH; i++){
		printf("simple_array[%d] = {%d, %ld, %d};\n", i, simple_array[i].x, simple_array[i].y, simple_array[i].z);
	}
	packed_simple_array	= create_packed_simple_item_array_initialized(ARR_LENGTH, 6, 7, 8);
	for(i = 0; i < ARR_LENGTH; i++){
		printf("packed_simple_array[%d] = {%d, %ld, %d};\n", i
		, packed_simple_array[i].x, packed_simple_array[i].y, packed_simple_array[i].z);
	}	
	free(simple_array);
	free(packed_simple_array);	
	
	simple_item_list *item_list	= create_simple_item_list();
	printf("Initial list count: %d size: %d\n", item_list->count, item_list->size);
	
	for(i = 0; i < 32; i++){
		add_item_simple_item_list(item_list, i, i*2, i*3);
		
		printf("Current list count: %d size: %d\n", item_list->count, item_list->size);		
		for(j = 0; j < item_list->count; j++){
			printf("\tlist_array[%d]: {%d,%ld,%d}\n", j, item_list->list_array[j].x
			, item_list->list_array[j].y, item_list->list_array[j].z);
		}
		
	}

	destroy_simple_item_list(item_list);	
}

