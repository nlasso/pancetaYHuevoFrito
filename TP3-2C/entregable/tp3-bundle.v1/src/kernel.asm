; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

global start


;; GDT
extern GDT_DESC

;; IDT
extern IDT_DESC
extern idt_inicializar

;; PIC
extern resetear_pic
extern habilitar_pic


;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

gdt:    dd 0x0 
        dd 0x0 

sc0: dd 0x00003FFF
     dd 0x00C09A00

sd0: dd 0x00003FFF
     dd 0x00C09200

video:  dd 0x80000FBF
        dd 0x0040920B

gdt_desc:   dw $-gdt
            dd gdt
;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0

    ; habilitar A20

    CALL habilitar_A20

    ; cargar la GDT
    
    LGDT [gdt_desc]

    ; setear el bit PE del registro CR0

    MOV eax, cr0
    OR eax, 1
    MOV cr0, eax
    JMP 0x8:modo_protegido

    ; pasar a modo protegido

BITS 32

modo_protegido:

    XOR eax, eax 
    MOV ax, 10000b
    MOV ds, ax
    MOV es, ax
    MOV gs, ax
    MOV ax, 11000b
    MOV fs, ax
    
    imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x07, 2, 0
    ; acomodar los segmentos

    ; setear la pila

    ; pintar pantalla, todos los colores, que bonito!

    ; inicializar el manejador de memoria

    ; inicializar el directorio de paginas

    ; inicializar memoria de tareas

    ; habilitar paginacion

    ; inicializar tarea idle

    ; inicializar todas las tsss

    ; inicializar entradas de la gdt de las tsss

    ; inicializar el scheduler

    ; inicializar la IDT

    ; configurar controlador de interrupciones

    ; cargar la tarea inicial

    ; saltar a la primer tarea

    ; Ciclar infinitamente (por si algo sale mal...)
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
