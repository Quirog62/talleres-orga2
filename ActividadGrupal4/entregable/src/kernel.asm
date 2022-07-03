; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TALLER System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "print.mac"

global start

extern GDT_DESC
extern screen_draw_layout

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
    ; <COMPLETAR>  1 línea
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font


    ; Imprimir mensaje de bienvenida
    print_text_rm start_rm_msg, start_rm_len, 0x07, 0, 0
    
    ; Habilitar A20
    call A20_disable
    call A20_check
    call A20_enable
    call A20_check

    ; Cargar la GDT  
    ; <COMPLETAR> ~ 1 linea 
    lgdt [GDT_DESC]  ; Ejercicio 9   

    ;  Setear el bit PE del registro CR0
    ; <COMPLETAR> ~ 3 lineas   
    mov edx, cr0
    or edx, 0x1
    mov cr0, edx 

    ; Saltar a modo protegido
    ; Hacemos un salto largo (far jump) y pasamos al codigo de modo protegido, 
    ; cargando los selectores de segmento de este modo
    ; Revisen que valor tiene la constante CS_RING_0_SEL e intuyan el por que.
	jmp CS_RING_0_SEL:modo_protegido

BITS 32
modo_protegido:
    ; A partir de aca, todo el codigo se va a ejectutar en modo protegido
    ; Establecer selectores de segmentos DS, ES, GS, FS y SS en el segmento de datos de nivel 0
    ; Pueden usar la constante DS_RING_0_SEL definida en este archivo
    ; <COMPLETAR> ~ 6 lineas   
    mov dx, DS_RING_0_SEL
    mov DS, dx 
    mov GS, dx
    mov FS, dx
    mov SS, dx
    mov ES, dx

    ; Establecer el tope y la base de la pila
    ; <COMPLETAR> ~ 2 lineas
    mov ebp, 0x25000
    mov esp, ebp
       
    ; Imprimir mensaje de bienvenida
    print_text_pm start_pm_msg, start_pm_len, 0x07, 4, 0

    ; Inicializar pantalla
    call screen_draw_layout

    
    ; Ciclar infinitamente 
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
