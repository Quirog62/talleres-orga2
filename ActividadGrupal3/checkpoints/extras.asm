SIMPLE_ITEM_LENGTH	EQU	24
SIMPLE_ITEM_Y_OFFSET	EQU	8
SIMPLE_ITEM_Z_OFFSET	EQU	16
PACKED_SIMPLE_ITEM_LENGTH	EQU	16
PACKED_SIMPLE_ITEM_Y_OFFSET	EQU	4
PACKED_SIMPLE_ITEM_Z_OFFSET	EQU	12

SIMPLE_ITEM_LIST_COUNT_OFFSET	EQU	4
SIMPLE_ITEM_LIST_LIST_OFFSET	EQU	8


;########### IMPORTED FUNCTIONS
extern malloc
extern realloc
extern free
extern fopen
extern fclose
extern fprintf

section .data


section .text

global create_simple_item_array
global create_packed_simple_item_array
global create_simple_item_array_initialized
global create_packed_simple_item_array_initialized
global add_item_simple_item_list

	
;########### FUNCTIONS DEFS
;simple_item *create_simple_item_array(uint32_t length);
create_simple_item_array:
	;prologue
	sub rsp, 8

	shl rdi, 32
	shr rdi, 32	
	mov rax, rdi	;mul will store the result in rdx:rax
	xor edx, edx	;clear upper part of mul destination
	mov r10, SIMPLE_ITEM_LENGTH	;move constant to register to apply mul
	mul r10		;rdx:rax <- rax * const
	mov rdi, rax	;move result back to first operand register
	call malloc WRT ..plt	;call function
	
	;epilogue
	add rsp, 8
	ret

;simple_item *create_simple_item_array_initialized(uint32_t length, uint32_t default_x
;	, uint64_t default_y, , uint32_t default_z);
create_simple_item_array_initialized:
	;prologue
	push rbx
	push r12
	push r13
	push r14
	sub rsp, 8

	mov ebx, edi	;move length to non volatile 
	mov r14d, esi	;move x to non volatile 
	mov r12, rdx	;move y to non volatile 
	mov r13d, ecx	;move z to non volatile 

	shl rdi, 32	;clear upper half of rdi
	shr rdi, 32	
	mov rax, rdi	;mul will store the result in rdx:rax
	xor edx, edx	;clear upper part of mul destination
	mov r10, SIMPLE_ITEM_LENGTH	;move constant to register to apply mul
	mul r10		;rdx:rax <- rax * const
	mov rdi, rax	;move result back to first operand register
	
	call malloc WRT ..plt	;call function
	
	xor rcx, rcx
	mov ecx, ebx	;initialize counter for loop
	
	mov rbx, rax	;set base of pointer
	
.cycle:	mov [rbx], r14d	;set default value
	mov [rbx + SIMPLE_ITEM_Y_OFFSET], r12
	mov [rbx + SIMPLE_ITEM_Z_OFFSET], r13d
	add rbx, SIMPLE_ITEM_LENGTH	;move pointer
	loop .cycle
	
	;epilogue
	add rsp, 8
	pop r14
	pop r13
	pop r12
	pop rbx
	ret


;extern packed_simple_item *create_packed_simple_item_array(uint32_t length);
create_packed_simple_item_array:
	;prologue
	sub rsp, 8
	
	shl rdi, 32
	shr rdi, 32	
	mov rax, rdi	;mul will store the result in rdx:rax
	xor edx, edx	;clear upper part of mul destination
	mov r10, PACKED_SIMPLE_ITEM_LENGTH	;move constant to register to apply mul
	mul r10		;rdx:rax <- rax * const
	mov rdi, rax	;move result back to first operand register
	call malloc WRT ..plt	;call function

	
	;epilogue
	add rsp, 8
	ret

;packed_simple_item *create_packed_simple_item_array_initialized(uint32_t length, uint32_t default_x
;	, uint64_t default_y, , uint32_t default_z);
create_packed_simple_item_array_initialized:
	;prologue
	push rbx
	push r12
	push r13
	push r14
	sub rsp, 8

	mov ebx, edi	;move length to non volatile 
	mov r14d, esi	;move x to non volatile 
	mov r12, rdx	;move y to non volatile 
	mov r13d, ecx	;move z to non volatile 

	shl rdi, 32
	shr rdi, 32	
	mov rax, rdi	;mul will store the result in rdx:rax
	xor edx, edx	;clear upper part of mul destination
	mov r10, PACKED_SIMPLE_ITEM_LENGTH	;move constant to register to apply mul
	mul r10		;rdx:rax <- rax * const
	mov rdi, rax	;move result back to first operand register
	call malloc WRT ..plt	;call function
	
	xor rcx, rcx
	mov ecx, ebx	;initialize conuter for loop
	
	mov rbx, rax	;set base of pointer
	
.cycle:	mov [rbx], r14d	;set default value
	mov [rbx + PACKED_SIMPLE_ITEM_Y_OFFSET], r12
	mov [rbx + PACKED_SIMPLE_ITEM_Z_OFFSET], r13d
	add rbx, PACKED_SIMPLE_ITEM_LENGTH	;move pointer
	loop .cycle
	
	;epilogue
	add rsp, 8
	pop r14
	pop r13
	pop r12
	pop rbx
	ret

;extern void add_item_simple_item_list(simple_item_list *list, uint32_t x, uint64_t y, uint32_t z);
add_item_simple_item_list:
	;prologue
	push rbx
	push r12
	push r13
	push r14
	push r15		

	mov r15, rdi	; list
	mov r12d, esi	; x
	mov r13, rdx	; y
	mov r14d, ecx	; z
	
	mov esi, [rdi + SIMPLE_ITEM_LIST_COUNT_OFFSET]
	mov eax, [rdi]
	inc esi
	cmp esi, eax		; compare list size with count
	jle .add_element	; if there is room to add an element do not resize
	
	shl DWORD [rdi], 1	; double the size of the list

	xor rax, rax
	mov eax, SIMPLE_ITEM_LENGTH	;mul will store the result in rdx:rax
	xor edx, edx	;clear upper part of mul destination
	xor r10, r10
	mov r10d, [rdi]	;move constant to register to apply mul
	mul r10		;rdx:rax <- rax * const

	xor rsi, rsi
	mov rsi, rax		; rdi is the new size (2nd parameter)
	; multiply with size of simple-item
	mov rdi, [rdi + SIMPLE_ITEM_LIST_LIST_OFFSET]	; rsi is the pointer to the array
	call realloc WRT ..plt
	
	mov rdi, r15	; we update rdi with the value of the new list
	mov [rdi + SIMPLE_ITEM_LIST_LIST_OFFSET], rax ; list->list_array = realloc(list->list_array, list->size)
	
.add_element:

	mov rbx, [rdi + SIMPLE_ITEM_LIST_LIST_OFFSET]	; rbx = list->list_array
	xor rax, rax
	mov eax, [rdi + SIMPLE_ITEM_LIST_COUNT_OFFSET]	;mul will store the result in rdx:rax
	xor edx, edx	;clear upper part of mul destination
	mov r10, SIMPLE_ITEM_LENGTH	;move constant to register to apply mul
	mul r10		;rdx:rax <- rax * const
	mov r10, rax	;move result back to first operand register
	add rbx, r10	;list->list_array[list->count]
	
	mov [rbx], r12d	; list->list_array[list->count].x = x;
	mov [rbx + SIMPLE_ITEM_Y_OFFSET], r13	; list->list_array[list->count].y = y;
	mov [rbx + SIMPLE_ITEM_Z_OFFSET], r14d	; list->list_array[list->count].z = z;

	inc DWORD [rdi + SIMPLE_ITEM_LIST_COUNT_OFFSET]	; list->count++
	
	;epilogue

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	ret
