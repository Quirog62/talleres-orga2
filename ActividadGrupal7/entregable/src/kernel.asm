; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TALLER System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "print.mac"

global start

extern GDT_DESC
extern IDT_DESC
extern idt_init
extern pic_reset
extern pic_enable
extern mmu_init
extern mmu_init_kernel_dir
extern mmu_init_task_dir
extern tss_init
extern task_init
extern screen_draw_layout
extern sched_init

%define EBP_INIT         0x0000
%define ESP_INIT         0x25000

%define CS_IDX_RING_0    1
%define DS_IDX_RING_0    2
%define VM_IDX_RING_0    5

%define CS_RING_0_SEL    (1 << 3)
%define DS_RING_0_SEL    (3 << 3)
%define VM_RING_0_SEL    (5 << 3)
%define TASK_INITIAL_SEL (11 << 3)
%define TASK_IDLE_SEL    (12 << 3)


BITS 16
;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
start_rm_msg db     'Iniciando kernel en Modo Real'
start_rm_len equ    $ - start_rm_msg

start_pm_msg db     'Iniciando kernel en Modo Protegido'
start_pm_len equ    $ - start_pm_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    print_text_rm start_rm_msg, start_rm_len, 0x07, 0, 0

    ;  Habilitar A20
    call A20_disable
    call A20_check
    call A20_enable
    call A20_check

    ; Cargar la GDT
    lgdt [GDT_DESC]

    ; Setear el bit PE del registro CR0
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax

    ; Saltar a modo protegido
	jmp CS_RING_0_SEL:modo_protegido

BITS 32
modo_protegido:
    ; Establecer selectores de segmentos
		mov ax, DS_RING_0_SEL
		mov ds, ax
		mov es, ax
		mov gs, ax
		mov ss, ax

    ; Establecer la base de la pila
		mov esp, ESP_INIT
		mov ebp, esp

    ; Imprimir mensaje de bienvenida
		print_text_pm start_pm_msg, start_pm_len, 0x07, 4, 0

    ; Inicializar pantalla
    call screen_draw_layout

	; Inicializar estructuras de mmu
	call mmu_init
    ; Inicializar el directorio de paginas
    call mmu_init_kernel_dir

    ; Cargar directorio de paginas
    mov cr3, eax

    ; Habilitar paginacion
    mov eax, cr0
    or eax, 1<<31
    mov cr0, eax

    ; Inicializar tss
    call tss_init
    
    ; Inicializar el scheduler
    call sched_init

    ; Inicializar las tareas
    call task_init

    ; Inicializar la IDT
    call idt_init

    ; Cargar IDT
    lidt [IDT_DESC]

    ; Configurar controlador de interrupciones
    call pic_reset
    call pic_enable

    ; Codigo para setear el intervalo del reloj.
    ; El valor en ax es un divisor de frecuencia.
    ; El PIT (Programmable Interrupt Timer) corre a 1193182Hz,
    ; Cada iteracion del clock decrementa un contador interno
    ; Cuando ese contador llega a 0, emite la interrupcion.
    ; Por defecto el valor inicial es 0x0 que se usa para indicar 65536
    ; Es decir, termina siendo 18.206 Hz.
    mov eax, 0x3000 ; esto da un tick de 97.1 Hz
    out 0x40, al 
    rol ax, 8
    out 0x40, al 
    ; Cargar tarea inicial
	; COMPLETAR
	mov bx, TASK_INITIAL_SEL
    LTR bx
    ; Saltar a la primera tarea: Idle
	; COMPLETAR
	JMP TASK_IDLE_SEL:0
    ; Ciclar infinitamente 
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
