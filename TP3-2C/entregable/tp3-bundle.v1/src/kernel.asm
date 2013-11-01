; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

global start


;; GDT
extern GDT_DESC

;;MMU
extern mmu_inicializar
extern mmu_inicializar_dir_tarea
extern mmu_inicializar_tareas
extern pd_tarea_idle
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

nombre_grupo_msg db         'Frambuesa a la crema'
nombre_grupo_len equ        $ - nombre_grupo_msg
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

    cli

    ; cargar la GDT

    LGDT [GDT_DESC]

    ; setear el bit PE del registro CR0

    MOV eax, cr0
    OR eax, 1
    MOV cr0, eax
    JMP 0x90:modo_protegido
    ; pasar a modo protegido

BITS 32

modo_protegido:

    ; acomodar los segmentos

    XOR eax, eax 
    MOV ax, 0xA0           ;cargo el selector de segmento de datos level 0: 20|0|00
    MOV ds, ax
    MOV es, ax
    MOV gs, ax
    MOV ss, ax
    MOV ax, 0xB0         ;cargo el selector de VIDEO: 22|0|00
    MOV fs, ax

    ; setear la pila

    MOV ebp, 0x27000
    MOV esp, ebp    

    imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x07, 2 , 0 
    ; pintar pantalla, todos los colores, que bonito!
        ;TODO
    ; inicializar el manejador de memoria

    limpiar_pantalla    ;esta macro limpia la pantalla.

    CALL mmu_inicializar

    ;Imprimo el nombre del grupo por pantalla
    imprimir_texto_mp nombre_grupo_msg, nombre_grupo_len, 0x07, 0, 1

    ; inicializar el directorio de paginas    

    ; inicializar memoria de tareas
    XCHG bx, bx
    CALL mmu_inicializar_dir_tarea
    ;CALL mmu_inicializar_tareas
    ; habilitar paginacion

    MOV eax, 0x27000
    MOV cr3, eax


    XCHG bx, bx
    
    MOV eax, cr0
    OR eax, 0x80000000
    MOV cr0, eax
XCHG bx, bx
    mov eax, [pd_tarea_idle]
    mov cr3, eax



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

