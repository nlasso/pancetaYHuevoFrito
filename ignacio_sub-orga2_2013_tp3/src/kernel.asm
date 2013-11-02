; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%define breakpoint xchg bx,bx
%define KERNEL_PAGE_DIR 0x0000 ; TODO: arreglar este macro

%include "imprimir.mac"

global start


;; GDT
extern GDT_DESC

;; MMU
extern mmu_inicializar
extern mmu_primeraPagina

;; IDT
extern IDT_DESC
extern idt_inicializar

;; PIC
extern resetear_pic
extern habilitar_pic

;; PANTALLA Y PRINTS
extern inicializar_pantalla
extern load_pantalla
extern cambiar_pantalla

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
  jmp 0x90:protec_mode

BITS 32
protec_mode:
    ;; ---------------------------------------------------------------------- ;;
    ;; acomodar los segmentos
    ;; ---------------------------------------------------------------------- ;;
    mov eax, 17*8 ; data segment 17
    mov ds, eax
    mov fs, eax
    mov es, eax
    mov gs, eax
    mov ss, eax

    ;; ---------------------------------------------------------------------- ;;
    ;; setear la pila
    ;; ---------------------------------------------------------------------- ;;
    mov ebp, 0x27000
    mov esp, 0x27000

    ;; ---------------------------------------------------------------------- ;;
    ;; pintar pantalla, todos los colores, que bonito!
    ;; ---------------------------------------------------------------------- ;;
    imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x07, 4, 3
    call inicializar_pantalla
    call load_pantalla
    breakpoint
    push byte 0 ;PREGUNTAR SI TENGO QUE POPPEAR ESTO
    ;call cambiar_pantalla
    pop byte eax
    call load_pantalla
    breakpoint ;sarasa
    

    ;; ---------------------------------------------------------------------- ;;
    ;; inicializar el manejador de memoria
    ;; ---------------------------------------------------------------------- ;;

    ;; ---------------------------------------------------------------------- ;;
    ;; inicializar el directorio de paginas
    ;; ---------------------------------------------------------------------- ;;
    call mmu_inicializar
    breakpoint
    ;; ---------------------------------------------------------------------- ;;
    ;; inicializar memoria de tareas
    ;; ---------------------------------------------------------------------- ;;

    ;; ---------------------------------------------------------------------- ;;
    ;; habilitar paginacion
    ;; ---------------------------------------------------------------------- ;;
    mov eax, 0x00027000
    mov cr3, eax
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    breakpoint

    ; inicializar tarea idle

    ; inicializar todas las tsss

    ; inicializar entradas de la gdt de las tsss

    ; inicializar el scheduler

    ; inicializar la IDT
    call idt_inicializar
    lidt[IDT_DESC]
    mov eax, 0
    div eax
    brekpoint

    mov eax, 0x00031000
    mov cr3, eax

    breakpoint

    ; configurar controlador de interrupciones

    ; cargar la tarea inicial

    ; saltar a la primer tarea
    
    ; Ciclar infinitamente (por si algo sale mal...)
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
