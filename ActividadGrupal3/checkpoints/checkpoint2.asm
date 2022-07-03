;########### ESTA ES LA FORMA DE DEFINIR CONSTANTES EN ASM (NOMBRE EQU VALOR)
;########### ESTOS SON LOS OFFSETS Y TAMAÑO DEL STRUCT ALINEADO
SIMPLE_ITEM_LENGTH	EQU	24
SIMPLE_ITEM_Z_OFFSET	EQU	16
;########### ESTOS SON LOS OFFSETS Y TAMAÑO DEL STRUCT EMPAQUETADO
PACKED_SIMPLE_ITEM_LENGTH	EQU	16
PACKED_SIMPLE_ITEM_Z_OFFSET	EQU	12


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
global alternate_sum_4
global alternate_sum_4_simplified
global alternate_sum_8
global sum_z
global packed_sum_z
global product_2
global product_2_f


;########### DEFINICION DE FUNCIONES
; uint32_t alternate_sum_4(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4);
; registros: x1[rdi], x2[rsi], x3[rdx], x4[rcx]
alternate_sum_4:
	;prologo
	;recordar que si la pila estaba alineada a 16 al hacer la llamada
	;con el push de RIP como efecto del CALL queda alineada a 8
	push rbp	; alineado a 16
	mov rbp, rsp
	push rbx	; alineado a 8
	push r12	; alineado a 16
	push r13	; alineado a 8
	push r14	; alineado a 16
	push r15	; alineado a 8
	sub rsp, 8	; compensamos para alinear a 16 (padding)
	
	xor rax, rax	; limpiamos rax
	add eax, edi	; realizamos las sumas alternadas
	sub eax, esi
	add eax, edx
	sub eax, ecx
	
	;epilogo
	add rsp, 8	; deshacemos el padding
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	
	pop rbp
	ret
	
; uint32_t alternate_sum_4_simplified(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4);
; registros: x1[rdi], x2[rsi], x3[rdx], x4[rcx]
alternate_sum_4_simplified:
	;esta versión está simplificada, no tiene epílogo ni prólogo
	;porque la función no toca registros no volátiles y no
	;realiza llamadas a funciones de libc (no recomendamos hacer esto
	;hasta no tener un muy buen manejo del lenguaje y las herramientas)
	
	xor rax, rax
	add eax, edi
	sub eax, esi
	add eax, edx
	sub eax, ecx
	
	ret

; uint32_t alternate_sum_8(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4, uint32_t x5, uint32_t x6, uint32_t x7, uint32_t x8);	
; registros y pila: x1[rdi], x2[rsi], x3[rdx], x4[rcx], x5[r8], x6[r9], x7[rbp + 0x10], x8[rbp + 0x18]
alternate_sum_8:
	;prologue
	; x8 <- RBP + 0x18 (24)
	; x7 <- RBP + 0x10 (16)
	; RIP <- RBP +0x08 | alineado a 8
	push rbp	; alineado a 16
	mov rbp, rsp
	push rbx	; <- RBP - 0x08 | alineado a 8
	push r12	; <- RBP - 0x10 |  alineado a 16
	push r13	; <- RBP - 0x18 | alineado a 8
	push r14	; <- RBP - 0x20 |  alineado a 16
	push r15	; <- RBP - 0x28 | alineado a 8

	sub rsp, 8	; agrega padding y alinea a 16
	
	lea rbx, [rbp + 0x10]	; rbx <- rbp + 16
	mov r10d, [rbx]		; r10d <- x7
	lea rbx, [rbp + 0x18]	; rbx <- rbp + 24
	mov r11d, [rbx]		; r11d <- x8
	
	xor rax, rax	; limpiamos rax
	add eax, edi	; sumamos todos los elementos
	sub eax, esi
	add eax, edx
	sub eax, ecx
	add eax, r8d
	sub eax, r9d
	add eax, r10d
	sub eax, r11d	; el resultado queda en eax
	
	
	;epilogue
	add rsp, 8
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rbp	
	ret
	
;uint32_t sum_z(simple_item *arr, uint32_t arr_length)
;registros: arr[rdi], arr_length[rsi]
sum_z:
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
.cycle:			; etiqueta a donde retorna el ciclo que itera sobre arr

			; rbx es la base que apunta al comienzo del arreglo (arr)
			; rsi es el desplazamiento que nos lleva al elemento actual (i * sizeof(simple_item))
			; SIMPLE_ITEM_Z_OFFSET es la ubicación del atributo respecto del comienzo del struct
	add eax, [rbx + rsi + SIMPLE_ITEM_Z_OFFSET]	; des referenciamos el puntero que apunta a la posicion
							; del atributo z del i-ésimo elemento
	add rsi, SIMPLE_ITEM_LENGTH			; avanzamos el registro de desplazamiento esi
	loop .cycle					; decrementa ecx y si es distinto de 0 salta a .cycle
	
	;epilogo
	add rsp, 8
	pop rbx
	pop rbp
	ret
	
;uint32_t packed_sum_z(simple_item *arr, uint32_t arr_length)
;registros: arr[rdi], arr_length[rsi]
packed_sum_z:
	;prologo
	push rbp	; alineado a 16
	mov rbp, rsp
	push rbx	; alineado a 8
	sub rsp, 8	; agregamos padding
	
	mov rbx, rdi
	xor rdi, rdi
	xor rax, rax
	xor rcx, rcx
	mov ecx, esi
	xor rsi, rsi
.cycle:
	add eax, [rbx + rsi + PACKED_SIMPLE_ITEM_Z_OFFSET];cómo difiere de la versión anterior?
	add rsi, PACKED_SIMPLE_ITEM_LENGTH		;cómo difiere de la versión anterior?	
	loop .cycle	

	;epilogo
	add rsp, 8
	pop rbx
	pop rbp
	ret
		
;void product_2(uint32_t * destination, uint32_t x1, uint32_t x2);
;registros: destination[rdi], x1[rsi], x2[rdx]
product_2:
	xor rax, rax	;limpiamos rax
	mov r9d, edx	;guardamos edx ya que la multiplicación se guarda en edx:eax
	xor rdx, rdx	;limpiamos rdx
	mov eax, esi	;cargamos eax para la multiplicación
	mul r9d		;edx:eax <- eax * r9d
	mov [rdi], eax	;guardamos el resultado en la posición apuntada por rdi
			;des referencia de destination
	ret
	
;void product_2_f(uint32_t * destination, uint32_t x1, float f1);
;registros: destination[rdi], x1[rsi], f1[xmm0]
product_2_f:
	xor rax, rax
	mov eax, esi		;limpiamos rax y cargamos x1 (esi ) en la parte baja
	cvtsi2sd xmm1, rax	;convertimos el escalar rax en un double en xmm1
	cvtss2sd xmm0, xmm0	;convertimos el float(32 bits) en xmm0 en un double(64 bits) en xmm0
	mulsd xmm0, xmm1	;multiplicamos los doubles xmm0 <- xmm0 * xmm1
	cvtsd2si rax, xmm0	;convetimos el double xmm0 en un scalar de 64 bits en rax
	mov [rdi], eax		;guardamos el resultado en la posición apuntada por rdi
				;des referencia de destination
				;¿qué problema hay con esta asignación?
	ret

		
