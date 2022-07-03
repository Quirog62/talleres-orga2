; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TALLER System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "print.mac"

global start

extern GDT_DESC
extern IDT_DESC
extern screen_draw_layout

extern idt_init
extern pic_reset
extern pic_enable

%define CS_RING_0_SEL    (1 << 3)
%define DS_RING_0_SEL    (3 << 3)


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

    print_text_rm start_rm_msg, start_rm_len, 0x07, 0, 0

    call A20_disable
    call A20_check
    call A20_enable
    call A20_check

    lgdt [GDT_DESC]

	mov eax, cr0
	or eax, 0x1
	mov cr0, eax

    ; Saltar a modo protegido
    ; Hacemos un salto largo (far jump) y pasamos al codigo de modo protegido, 
    ; cargando los selectores de segmento de este modo
	jmp CS_RING_0_SEL:modo_protegido

BITS 32
modo_protegido:
    mov ax, DS_RING_0_SEL 
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
	
    ; Establecer el tope y la base de la pila
    mov esp, 0x00025000
    mov ebp, esp  

    ; Imprimir mensaje de bienvenida
    print_text_pm start_pm_msg, start_pm_len, 0x07, 4, 0

    ; Inicializar pantalla
    call screen_draw_layout

    ; Configurar controlador de interrupciones

    call pic_reset
    call pic_enable

    ; -- COMPLETAR --
    
    ; -- CHECKPOINT 2 --

    ; inicializar IDT
    ; Cargar IDT
	CALL idt_init
	LIDT [IDT_DESC]
    ; ------------------

    ; -- CHECKPOINT 3 --

    ; habilitar interrupciones
    ; Opcional: hacer una división por cero para probar o usar la instrucción INT (RECORDAR LUEGO BORRAR o COMENTAR LA PRUEBA)
	STI	
	INT $88 
    ; ------------------

    ; Ciclar infinitamente 
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
