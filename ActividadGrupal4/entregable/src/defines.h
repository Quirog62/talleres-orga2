/* ** por compatibilidad se omiten tildes **
================================================================================
 TALLER System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Misc */
/* -------------------------------------------------------------------------- */
// Y Filas
#define SIZE_N 40
#define ROWS   SIZE_N

// X Columnas
#define SIZE_M 80
#define COLS   SIZE_M

/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_IDX_NULL_DESC 0
#define GDT_COUNT         35

#define GDT_IDX_CODE_0 1
#define GDT_IDX_CODE_3 2
#define GDT_IDX_DATA_0 3
#define GDT_IDX_DATA_3 4
#define GDT_IDX_VIDEO  5


/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC (GDT_IDX_NULL_DESC << 3)
#define GDT_OFF_VIDEO  (GDT_IDX_VIDEO << 3)
#define GDT_OFF_CODE_3 (GDT_IDX_CODE_3 << 3)
#define GDT_OFF_DATA_3 (GDT_IDX_DATA_3 << 3)

/* COMPLETAR - Valores para los selectores de segmento de la GDT */
/* Notar que << hace un shift a la izquierda una cantidad X de bits */
/* en este caso, esta shifteando 3 bits, ¿por que? */
#define GDT_CODE_0_SEL (GDT_IDX_CODE_0 << 3)
#define GDT_DATA_0_SEL (GDT_IDX_DATA_0 << 3)
/* | simboliza el OR en bits. Dado que es nivel 3 precisamos que los últimos 2 bits sea el privilegio 0x3*/
#define GDT_CODE_3_SEL ((GDT_OFF_CODE_3) | 0x0003 )
#define GDT_DATA_3_SEL ((GDT_OFF_DATA_3) | 0x0003 )


/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */

// direccion fisica de comienzo del bootsector (copiado)
#define BOOTSECTOR 0x00001000
// direccion fisica de comienzo del kernel
#define KERNEL 0x00001200
// direccion fisica del buffer de video
#define VIDEO 0x000B8000


#endif //  __DEFINES_H__
