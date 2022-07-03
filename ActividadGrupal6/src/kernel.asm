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
extern mmu_init
extern mmu_init_kernel_dir
extern mmu_init_task_dir
extern print_hex

%define CS_RING_0_SEL    (1 << 3)
%define DS_RING_0_SEL    (3 << 3)
%define TASK_CODE_PHYS_A 0x00018000


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
    ; Hacemos un salto largo (far jump) y pasamos al codigo de modo protegido, 
    ; cargando los selectores de segmento de este modo
    ; Revisen que valor tiene la constante CS_RING_0_SEL e intuyan el por que.
	jmp CS_RING_0_SEL:modo_protegido

BITS 32
modo_protegido:
    ; A partir de aca, todo el codigo se va a ejectutar en modo protegido
    ; Establecer selectores de segmentos DS, ES, GS, FS y SS en el segmento de datos de nivel 0
    ; Pueden usar la constante DS_RING_0_SEL definida en este archivo
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

    ; Inicializar el manejador de memoria
    call mmu_init

    ; Inicializar el directorio de paginas   ;COMPLETAR:

    ; Cargar directorio de paginas  ;COMPLETAR:

    ; Habilitar paginacion  ;COMPLETAR:    
    xchg bx, bx
	call mmu_init_kernel_dir
    mov ebx, 0x25000
    mov cr3, ebx

    mov edx, cr0
    or edx, 0x80000001
    mov cr0, edx
    
    xchg bx, bx 

    ; Quiten este jump una vez que terminen mmu_init_task_dir     
    ; Para poder probar el nuevo CR3; COMPLETAR:
    ; Probar estructuras de paginacion para tareas
    ; Preservamos el CR3 del kernel
    mov eax, cr3
    ; Inicializamos las estructuras de paginacion para
    ; la tarea cuyo codigo se encuentra en TASK_CODE_PHYS_A
    push eax
    push TASK_CODE_PHYS_A
    call mmu_init_task_dir
    add esp, 4
    ; Cargamos el nuevo valor de CR3 (el de la tarea)
    mov cr3, eax
    ; Imprimimos los caracteres de prueba
    ;void print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y, uint16_t attr);
    ; Pasamos los parametros por pila (convencion 32 bits)
    push 0x0F
    push 0
    push 0
    push 2
    push 0xFF
    call print_hex
    add esp, 20
    ; En este breakpoint info tab deberia mostrar el mapeo de la tarea
    xchg bx, bx
    pop eax    
    mov cr3, eax
        ; En este breakpoint info tab deberia mostrar el mapeo del kernel
    xchg bx, bx
cargar_interrupciones:
    call idt_init
    ; Cargar IDT
    lidt [IDT_DESC]

    ; Configurar controlador de interrupciones

    call pic_reset
    call pic_enable

    ; habilitar interrupciones
    sti

    ;mov edx, 0
    ;div edx
    
    ; Ciclar infinitamente 
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
