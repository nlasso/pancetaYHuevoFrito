; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================


;%define KERNEL_PAGE_DIR 0x0000 ; TODO: arreglar este macro
%include "defines.asm"
%include "imprimir.mac"

global start


;; GDT
extern GDT_DESC

;; MMU
extern mmu_inicializar
extern mmu_primeraPagina
extern mmu_tareas_a_mar

;; IDT
extern IDT_DESC
extern idt_inicializar

;; PIC
extern deshabilitar_pic
extern resetear_pic
extern habilitar_pic

;; PANTALLA Y PRINTS
extern screen_en_negro
extern inicializar_pantalla
extern inicializar_pantalla_memoria
extern load_pantalla
extern cambiar_pantalla

;; TSS 
extern tss_inicializar
extern tarea_inicial
extern gdt_set_tss

;;SCHED
extern sched_inicializar

;;TEST  STUFF
extern reubicar_pagina;
extern TASK_CR3;

;; Saltear seccion de datos
jmp start

;; -------------------------------------------------------------------------- ;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

;; -------------------------------------------------------------------------- ;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;
;;     Punto de entrada del kernel.
;; -------------------------------------------------------------------------- ;;
BITS 16
start:
  ;; ------------------------------------------------------------------------ ;;
  ;; Deshabilitar interrupciones
  ;; ------------------------------------------------------------------------ ;;
  cli
  ;; ------------------------------------------------------------------------ ;;
  ;; Imprimir mensaje de bienvenida
  ;; ------------------------------------------------------------------------ ;;
  imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 3 , 1
  ;; ------------------------------------------------------------------------ ;;
  ;; habilitar A20
  ;; ------------------------------------------------------------------------ ;;
  call habilitar_A20
  cli
  ;; ------------------------------------------------------------------------ ;;
  ;; cargar la GDT
  ;; ------------------------------------------------------------------------ ;;
  lgdt [GDT_DESC]
  ;; ------------------------------------------------------------------------ ;;
  ;; setear el bit PE del registro CR0
  ;; ------------------------------------------------------------------------ ;;
  mov eax, cr0
  or eax, 1
  mov cr0, eax
  ;; ------------------------------------------------------------------------ ;;
  ;; pasar a modo protegido
  ;; ------------------------------------------------------------------------ ;;
  jmp GDT_CODE_0:Modo_protegido

BITS 32
Modo_protegido:
    ;; ---------------------------------------------------------------------- ;;
    ;; acomodar los segmentos
    ;; ---------------------------------------------------------------------- ;;
    mov eax, GDT_DATA_0 ; data segment 17
    mov ds, eax
    mov fs, eax
    mov es, eax
    mov gs, eax
    mov ss, eax
   ; breakpoint

    ;; ---------------------------------------------------------------------- ;;
    ;; setear la pila
    ;; ---------------------------------------------------------------------- ;;
    mov ebp, PILAKERNEL
    mov esp, PILAKERNEL

    ;; ---------------------------------------------------------------------- ;;
    ;; pintar pantalla, todos los colores, que bonito!
    ;; ---------------------------------------------------------------------- ;;
    imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x07, 4, 3

    call screen_en_negro
    call inicializar_pantalla
    ;call load_pantalla
    ;breakpoint
    ;push byte 0 ;PREGUNTAR SI TENGO QUE POPPEAR ESTO
    ;call cambiar_pantalla
    ;pop byte eax
    ;call load_pantalla
    
    

    ;; ---------------------------------------------------------------------- ;;
    ;; inicializar el manejador de memoria
    ;; ---------------------------------------------------------------------- ;;

    ;; ---------------------------------------------------------------------- ;;
    ;; inicializar el directorio de paginas
    ;; ---------------------------------------------------------------------- ;;
    call mmu_inicializar
    call inicializar_pantalla
    ;; ---------------------------------------------------------------------- ;;
    ;; imprimir el directorio de paginas
    ;; ---------------------------------------------------------------------- ;;
    call inicializar_pantalla_memoria
    ;; ---------------------------------------------------------------------- ;;
    ;; inicializar memoria de tareas
    ;; ---------------------------------------------------------------------- ;;
 

    ;; ---------------------------------------------------------------------- ;;
    ;; habilitar paginacion
    ;; ---------------------------------------------------------------------- ;;
    mov eax, [TASK_CR3]
    mov cr3, eax
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    ; inicializar tarea idle

    ; inicializar todas las tsss
    call gdt_set_tss
    call tss_inicializar


    ; inicializar entradas de la gdt de las tsss
    ;CALL gdt_set_tss

    ; inicializar el schedule
 
    mov eax, [TASK_CR3+4]
    mov cr3, eax
    mov eax, [TASK_CR3]
    mov cr3, eax


    CALL sched_inicializar

    ;breakpoint

    ; inicializar la IDT
    call idt_inicializar
    lidt [IDT_DESC]
    call deshabilitar_pic
    call resetear_pic
    call habilitar_pic

    

    mov ax, GDT_INICIAL
    ltr ax
    mov ax, ax
    jmp GDT_IDLE:0x0
    ;breakpoint
    ;mov eax, 0
    ;div eax
    ;breakpoint
    ;int 12
    ;breakpoint
;
    ;mov eax, 0x00032000
    ;mov cr3, eax
;
    ;breakpoint
;
    ;mov eax, 1
    ;div eax
;
;breakpoint
    ; configurar controlador de interrupciones

    ; cargar la tarea inicial

    ; saltar a la primer tarea
    sti
    ; Ciclar infinitamente (por si algo sale mal...)
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
