/* ** por compatibilidad se omiten tildes **
================================================================================
 TALLER System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* MMU */
/* -------------------------------------------------------------------------- */
#define VIRT_PAGE_OFFSET(X) ((X)&0xFFF)
#define VIRT_PAGE_TABLE(X)  (((X) >> 12) & 0x03FF)
#define VIRT_PAGE_DIR(X)    (((X) >> 22) & 0x03FF)
#define CR3_TO_PAGE_DIR(X)  ((X) & (~0xFFF))
#define MMU_ENTRY_PADDR(X)  ((X) << 12)

#define MMU_P (1 << 0)
#define MMU_W (1 << 1)
#define MMU_U (1 << 2)

#define PAGE_SIZE 4096

// direccion virtual del codigo
#define TASK_CODE_VIRTUAL 0x08000000
#define TASK_CODE_PAGES   2
#define TASK_STACK_BASE   0x08003000

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */
#define KERNEL_PAGE_DIR     (0x00025000)
#define KERNEL_PAGE_TABLE_0 (0x00026000)
#define KERNEL_STACK        (0x00025000)

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

/* COMPLETAR - Valores para los selectores de segmento de la GDT */
/* Notar que << hace un shift a la izquierda una cantidad X de bits */
/* en este caso, esta shifteando 3 bits, ¿por que? */
#define GDT_CODE_0_SEL (GDT_IDX_CODE_0 << 3)
#define GDT_DATA_0_SEL (GDT_IDX_DATA_0 << 3)
/* | simboliza el OR en bits. Dado que es nivel 3 precisamos que los últimos 2 bits sea el privilegio 0x3*/
#define GDT_CODE_3_SEL ((GDT_OFF_CODE_3) | 0x3 )
#define GDT_DATA_3_SEL ((GDT_OFF_DATA_3) | 0x3 )

// Macros para trabajar con segmentos de la GDT.

// SEGM_LIMIT_4KIB es el limite de segmento visto como bloques de 4KIB
// principio del ultimo bloque direccionable.
#define GDT_LIMIT_4KIB(X)  (((X) / 4096) - 1)
#define GDT_LIMIT_BYTES(X) ((X)-1)

#define GDT_LIMIT_LOW(limit)  (uint16_t)(((uint32_t)(limit)) & 0x0000FFFF)
#define GDT_LIMIT_HIGH(limit) (uint8_t)((((uint32_t)(limit)) >> 16) & 0x0F)

#define GDT_BASE_LOW(base)  (uint16_t)(((uint32_t)(base)) & 0x0000FFFF)
#define GDT_BASE_MID(base)  (uint8_t)((((uint32_t)(base)) >> 16) & 0xFF)
#define GDT_BASE_HIGH(base) (uint8_t)((((uint32_t)(base)) >> 24) & 0xFF)

#define DESC_TYPE_EXECUTE_READ 0xA
#define DESC_TYPE_READ_WRITE   0x2
#define DESC_TYPE_32BIT_TSS    0x9

#define DESC_CODE_DATA 1
#define DESC_SYSTEM    0

#define FLAT_SEGM_SIZE  (817U * (1 << 20))
#define VIDEO_SEGM_SIZE (80U * 50 * 2)

#define INTERRUPT_GATE_0_ATTRS 0x8E00
#define INTERRUPT_GATE_3_ATTRS 0xEE00
/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */

// direccion fisica de comienzo del bootsector (copiado)
#define BOOTSECTOR 0x00001000
// direccion fisica de comienzo del kernel
#define KERNEL 0x00001200
// direccion fisica del buffer de video
#define VIDEO 0x000B8000


#endif //  __DEFINES_H__
