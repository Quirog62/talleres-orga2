;########### ESTA ES LA FORMA DE DEFINIR CONSTANTES EN ASM (NOMBRE EQU VALOR)
;########### ESTOS SON LOS OFFSETS Y TAMAÑO DEL STRUCT ALINEADO
COMPLEX_ITEM_LENGTH	EQU	28;?? completar valor
COMPLEX_ITEM_Z_OFFSET	EQU	24 ;?? completar valor
;########### ESTOS SON LOS OFFSETS Y TAMAÑO DEL STRUCT EMPAQUETADO
;PACKED_COMPLEX_ITEM_LENGTH	EQU	0 ;?? completar valor
;PACKED_COMPLEX_ITEM_Z_OFFSET	EQU	0 ;?? completar valor


;########### LISTA DE FUNCIONES IMPORTADAS
extern malloc
extern free
extern fopen
extern fclose
extern fprintf

;########### SECCION DE DATOS
section .data

;########### SECCION DE TEXTO (PROGRAMA)
section .text

;########### LISTA DE FUNCIONES EXPORTADAS
global complex_sum_z
global packed_complex_sum_z
global product_9_f



;########### DEFINICION DE FUNCIONES
;extern uint32_t complex_sum_z(complex_item *arr, uint32_t arr_length);
;registros: arr[rdi], arr_length[rsi]
complex_sum_z:
	;prologo
	push rbp	; alineado a 16
	mov rbp, rsp
	push rbx	; alineado a 8
	sub rsp, 8	; agregamos padding
	
	mov rbx, rdi	; rbx <- arr
	xor rdi, rdi	; limpiamos rdi
	xor rax, rax	; limpiamos rax
	xor rcx, rcx	; limpiamos rcx para usarlo con loop
	mov ecx, esi	; copiamos los 32 bits más bajos de esi (arr_length) en ecx
	xor rsi, rsi	; limpiamos rsi 
			; observar que no utilizamos registros no volátiles
			; caso contrario tendríamos que haberlos preservado
;COMPLETAR:
	loop .cycle:	; etiqueta a donde retorna el ciclo que itera sobre arr

			; rbx es la base que apunta al comienzo del arreglo (arr)
			; rsi es el desplazamiento que nos lleva al elemento actual (i * sizeof(simple_item))
			; ? es la ubicación del atributo respecto del comienzo del struct
;	add eax, [rbx + rsi + ?]	; des referenciamos el puntero que apunta a la posicion
;					; del atributo z del i-ésimo elemento
;	add rsi, ?			; avanzamos el registro de desplazamiento esi
;	loop .cycle			; decrementa ecx y si es distinto de 0 salta a .cycle

	
	;epilogo
	add rsp, 8
	pop rbx
	pop rbp
	ret
	
;extern uint32_t packed_complex_sum_z(packed_complex_item *arr, uint32_t arr_length);
;registros: arr[rdi], arr_length[rsi]
packed_complex_sum_z:
	;prologo
	push rbp	; alineado a 16
	mov rbp, rsp
	push rbx	; alineado a 8
	sub rsp, 8	; agregamos padding
	
	mov rbx, rdi	; rbx <- arr
	xor rdi, rdi	; limpiamos rdi
	xor rax, rax	; limpiamos rax
	xor rcx, rcx	; limpiamos rcx para usarlo con loop
	mov ecx, esi	; copiamos los 32 bits más bajos de esi (arr_length) en ecx
	xor rsi, rsi	; limpiamos rsi 
			; observar que no utilizamos registros no volátiles
			; caso contrario tendríamos que haberlos preservado
;COMPLETAR:
;.cycle:		; etiqueta a donde retorna el ciclo que itera sobre arr

			; rbx es la base que apunta al comienzo del arreglo (arr)
			; rsi es el desplazamiento que nos lleva al elemento actual (i * sizeof(simple_item))
			; ? es la ubicación del atributo respecto del comienzo del struct
;	?				; des referenciamos el puntero que apunta a la posicion
;					; del atributo z del i-ésimo elemento
;	?				; avanzamos el registro de desplazamiento esi
;	?				; decrementa ecx y si es distinto de 0 salta a .cycle
	
	;epilogo
	add rsp, 8
	pop rbx
	pop rbp
	ret


;extern void product_9_f(uint32_t * destination
;, uint32_t x1, float f1, uint32_t x2, float f2, uint32_t x3, float f3, uint32_t x4, float f4
;, uint32_t x5, float f5, uint32_t x6, float f6, uint32_t x7, float f7, uint32_t x8, float f8
;, uint32_t x9, float f9);
;registros y pila: destination[rdi], x1[rsi], f1[xmm0], x2[rdx], f2[xmm1], x3[rcx], f3[xmm2], x4[r8], f4[xmm3]
;	, x5[r9], f5[xmm4], x6[rbp+0x10], f6[xmm5], x7[rbp + 0x18], f7[?], x8[?], f8[?],
;	, x9[?], f9[?]
product_9_f:
	push rbp
	mov rbp, rsp
	
	cvtss2sd xmm0, xmm0	;convertimos los flotantes de cada registro xmm en doubles
	cvtss2sd xmm1, xmm1	
	cvtss2sd xmm2, xmm2	
	cvtss2sd xmm3, xmm3	
	cvtss2sd xmm4, xmm4	
;	cvtss2sd ?,?
;	cvtss2sd ?,?
;	cvtss2sd ?,?
	mulsd xmm0, xmm1	;multiplicamos los doubles xmm0 <- xmm0 * xmm1
	mulsd xmm0, xmm2
	mulsd xmm0, xmm3
	mulsd xmm0, xmm4
;	mulsd xmm0, ?
;	mulsd xmm0, ?
;	mulsd xmm0, ?
;	cvtss2sd xmm1, [?]	;convertimos el float que falta desde memoria (f9)
	mulsd xmm0, xmm1	;multiplicamos los doubles xmm0 <- xmm0 * xmm1	
	xor rax, rax
	mov eax, esi		;limpiamos rax y cargamos x1 (esi ) en la parte baja
	cvtsi2sd xmm1, rax	;convertimos el escalar rax en un double en xmm1	
	mulsd xmm0, xmm1
	mov eax, edx		;limpiamos rax y cargamos x2 (edx ) en la parte baja
	cvtsi2sd xmm1, rax	;convertimos el escalar rax en un double en xmm1	
	mulsd xmm0, xmm1
	mov eax, ecx		
	cvtsi2sd xmm1, rax	
	mulsd xmm0, xmm1
	mov eax, r8d		
	cvtsi2sd xmm1, rax	
	mulsd xmm0, xmm1	
	mov eax, r9d		
	cvtsi2sd xmm1, rax	
	mulsd xmm0, xmm1	
;	mov eax, [?]		;x6
;	cvtsi2sd xmm1, rax	
;	mulsd xmm0, xmm1	
;	mov eax, [?]		;x7
;	cvtsi2sd xmm1, rax	
;	mulsd xmm0, xmm1		
;	mov eax, [?]		;x8
;	cvtsi2sd xmm1, rax	
;	mulsd xmm0, xmm1
;	mov eax, [?]		;x9
;	cvtsi2sd xmm1, rax	
;	mulsd xmm0, xmm1

	movq [rdi], xmm0	
	
	pop rbp
	ret

