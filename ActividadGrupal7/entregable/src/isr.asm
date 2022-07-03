; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TALLER System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
;
; Definicion de rutinas de atencion de interrupciones

%include "print.mac"
%define DS_RING_0_SEL     (3 << 3)
%define CS_RING_0_SEL     (1 << 3)
%define GDT_TASK_IDLE_SEL (12 << 3)
%define TICKS_TASK_SWITCH 200
BITS 32

sched_task_offset:     dd 0xFFFFFFFF
sched_task_selector:   dw 0xFFFF

;; PIC
extern pic_finish1

;; Sched
extern sched_next_task
extern sched_current_task
extern tick_count
extern task_tick
extern kernel_exception
extern user_exception
extern disable_task
extern page_fault_handler

;; MMU
extern mmu_getshm
extern mmu_freeshm

;; SCREEN
extern print
extern print_dec
extern print_hex

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;
%macro ISR 1
global _isr%1

_isr%1:
    mov eax, %1
    jmp $

%endmacro

%macro ISRc 1
    push DWORD %1
    ; Stack State:
    ; [ INTERRUPT #] esp
    ; [ ERROR CODE ] esp + 0x04
    ; [ EIP        ] esp + 0x08
    ; [ CS         ] esp + 0x0c
    ; [ EFLAGS     ] esp + 0x10
    ; [ ESP        ] esp + 0x14 (if DPL(cs) == 3)
    ; [ SS         ] esp + 0x18 (if DPL(cs) == 3)

    ; GREGS
    pushad
    mov eax, DS_RING_0_SEL
    mov ds, ax

    ; Check for privilege change before anything else.
    mov edx, [esp + (8*4 + 3*4)]

    ; SREGS
    xor eax, eax
    mov ax, ss
    push eax
    mov ax, gs
    push eax
    mov ax, fs
    push eax
    mov ax, es
    push eax
    mov ax, ds
    push eax
    mov ax, cs
    push eax 

    ; CREGS
    mov eax, cr4
    push eax
    mov eax, cr3
    push eax
    mov eax, cr2
    push eax
    mov eax, cr0
    push eax
    extern current_task
    push DWORD [current_task]

    cmp edx, CS_RING_0_SEL
    je .ring0_exception

.ring3_exception:  
    call user_exception
    call disable_task
    call jmp_idle_task
    jmp $


.ring0_exception:  
    call kernel_exception
    add esp, 11*4
    popad
    
    xchg bx, bx
    jmp $

%endmacro

; ISR that pushes an exception code.
%macro ISRE 1
global _isr%1

_isr%1:
  ISRc %1
%endmacro

; ISR That doesn't push an exception code.
%macro ISRNE 1
global _isr%1

_isr%1:
  push DWORD 0x0
  ISRc %1
%endmacro

;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISRNE 0
ISRNE 1
ISRNE 2
ISRNE 3
ISRNE 4
ISRNE 5
ISRNE 6
ISRNE 7
ISRE 8
ISRNE 9
ISRE 10
ISRE 11
ISRE 12
ISRE 13
ISRE 14
ISRNE 15
ISRNE 16
ISRE 17
ISRNE 18
ISRNE 19
ISRNE 20

;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
global _isr32

_isr32:
    pushad
    call pic_finish1
    call next_clock
	; COMPLETAR
    ; quiten la siguiente linea para poner en ejecucion el scheduler
    call task_tick

    mov ecx, [tick_count]
    cmp ecx, TICKS_TASK_SWITCH
    jne .fin

    mov dword [tick_count], 0
    call sched_next_task
    cmp ax, 0
    je .fin

    str bx
    cmp ax, bx
    je .fin

    mov word [sched_task_selector], ax
    jmp far [sched_task_offset]
    ;Esta leyendo una dirección logica de 48 bits.
    .fin:
    popad
    iret
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
global _isr33

_isr33:
    pushad
    call pic_finish1
    popad
    iret
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;

; PushAD Order
%define offset_EAX 28
%define offset_ECX 24
%define offset_EDX 20
%define offset_EBX 16
%define offset_ESP 12
%define offset_EBP 8
%define offset_ESI 4
%define offset_EDI 0


global _isr88 ; getshm

_isr88:
  pushad
  push eax
  call mmu_getshm
  add esp, 4
  mov [esp + offset_EAX], eax
  popad
  iret

global _isr98 ; freeshm

_isr98:
  pushad
  push eax
  call mmu_freeshm
  add esp, 4
  popad
  iret

global _isr108 ; get_id

_isr108:
  pushad
  call sched_current_task
  mov [esp + offset_EAX], eax
  popad
  iret

global _isr118 ; print

_isr118:
  pushad
  push edx
  push ecx
  push ebx
  push eax
  call print
  add esp, 16
  popad
  iret

global _isr128 ; print_dec

_isr128:
  pushad
  push edx
  push ecx
  push ebx
  push eax
  call print_dec
  add esp, 16
  popad
  iret

global _isr138 ; print_hex

_isr138:
  pushad
  push edx
  push ecx
  push ebx
  push eax
  call print_hex
  add esp, 16
  popad
  iret

jmp_idle_task:
  mov ax, GDT_TASK_IDLE_SEL
  mov word [sched_task_selector], ax
  jmp far [sched_task_offset]

;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
isrNumber:           dd 0x00000000
isrClock:            db '|/-\'
next_clock:
        pushad
        inc DWORD [isrNumber]
        mov ebx, [isrNumber]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrNumber], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                print_text_pm ebx, 1, 0x0f, 49, 79
                popad
        ret
