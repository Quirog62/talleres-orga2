#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "checkpoints.h"


/*
Compilación condicional (leer cuando terminen los checkpoints):

Cuando terminen con los checkpoints pueden leer las funciones extras
en extras.asm y ejecutarlas asignando un 1 a EJECUTAR_EXTRAS
la directiva #if ... #endif se puede utilizar para compilar bloques de
código en base a condiciones definidas en constantes definidas de este
modo.
*/
#define EJECUTAR_EXTRAS	0


int main (void){
	//COMPLETAR:
	//Lean y expliquen las funciones de checkpoint2.c y checkpoint2.asm
	checkpoint_2();
	//COMPLETAR:	
	//Lean las funciones de checkpoint3.c y completen las partes de
	//código necesairas en checkpoint3.asm	
	//checkpoint_3();	

#if EJECUTAR_EXTRAS
	//No le preseten atención a extras.c y extras.asm salvo que hayan corregido
	//todos los checkpoints, no vamos a responder consultas de esta parte de código
	//mientras haya grupos que tengan que corregir checkpoints, es para que lean
	//si tienen ganas quienes terminaron.
	extras();
#endif
	return 0;    
}


