; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"
%include "defines.asm"

BITS 32


;; PIC
extern fin_intr_pic1

;; SCREEN 
extern load_pantalla
extern cambiar_pantalla

;; MMU
extern mmu_mapear_pagina
extern canionear
extern navegar

;;SCHED
extern restar_quantum
extern dame_tarea_actual
extern desalojar_tarea
extern saltar_idle

;;SCREEN ERROR RELATED
extern print_error
extern print_tablaerror
extern estado_error

;;
;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;

global jump_idle
jump_idle:
    pushad
    jmp GDT_IDLE:0x0
    popad
    iret


;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
    ;cli
    pushad
    ;breakpoint
    mov eax, 1
    mov [estado_error], eax
    mov ax, 2
    mov [estado_error+4], eax
    mov ax, 3
    mov [estado_error+8], eax
    mov ax, 4
    mov [estado_error+12], eax
    mov ax, 5
    mov [estado_error+16], eax
    mov ax, 6
    mov [estado_error+20], eax
    mov ax, 7
    mov [estado_error+24], eax
    mov ax, 8
    mov [estado_error+28], eax
    mov ax, 9
    mov [estado_error+32], eax
    mov ax, 10
    mov [estado_error+36], eax
    mov ax, 11
    mov [estado_error+40], eax
    mov ax, 12
    mov [estado_error+44], eax
    mov ax, 13
    mov [estado_error+48], eax
    mov ax, 14
    mov [estado_error+52], eax
    mov ax, 15
    mov [estado_error+56], eax
    mov ax, 16
    mov [estado_error+60], eax
    mov ax, 17
    mov [estado_error+64], eax
    mov ax, 18
    mov [estado_error+68], eax
    mov ax, 19
    mov [estado_error+72], eax
    call print_tablaerror;
    xor ax, ax
    mov ax, %1
    push ax
    call print_error
    pop  ax
    call desalojar_tarea
    call saltar_idle        ;VERIFICAR: esto. Debería saltar en cualquier error a IDLE.
    popad
    CALL load_pantalla;
    sti
    breakpoint
    ; To Infinity And Beyond!!
    iret
    ;jmp $
%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler
reloj_numero:           dd 0x00000000
reloj:                  db '|/-\'
numeros_msj:            db '1234567890'
numeros_len equ         $ - numeros_msj
segsel


;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19


;;
;;Rutina de atencion de interrupcion invalida
;;--------------------------------------------------------------------------- ;;
global int_invalida
int_invalida:
    cli
    pushad
    CALL fin_intr_pic1
    CALL print_error
    CALL desalojar_tarea
    CALL saltar_idle
    popad
    sti
    iret

;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
global screen_proximo_reloj
screen_proximo_reloj:
    cli
    pushad
    CALL fin_intr_pic1
    CALL proximo_reloj
    CALL restar_quantum              ;Decremento en uno el QUANTUM_RESTANTE

    popad
    sti
    ret

;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
global int_teclado
int_teclado:
    ;breakpoint
    cli
    pushad
    in al, 0x60             ;recibo la la lectura del teclado. 
    mov ah, al
    or al, 0x80             ;seteo el ultimo flag del byte entonces asi le digo al status que me de otra y que ya procese esta.
    out 0x61, al            ;Lo envio al controlador de teclado
    mov al, ah
    out 0x61, al            ;Vuelvo a como estaba antes el controlador.
    and ah, 0x80            ;Veo si ya solto la tecla sino termino la interrupcion.
    jnz fin_teclado
solto_tecla:
    cmp al, 0x32            ;= 'm'
    je activar_mapa
    cmp al, 0x12            ;= 'e'
    je activar_estado
    mov cl, 0x02
    cmp al, 0x0B
    je print_cero
ciclo_numero:               ;los numeros en el scan code van del 0x02 al 0x0B entonces hago un ciclo para ver si está en ese rango
    cmp al, cl              
    je print_number
    add cl, 1               ; sumo el contador del rango en uno.
    cmp cl, 0x0C
    je fin_teclado
    jmp ciclo_numero

activar_mapa:
    ;mov edi, SCREENMAPA         ;cambio la pantalla a mapa
    push SCREENMAPA
    call cambiar_pantalla
    call load_pantalla
    pop edi
    jmp fin_teclado

activar_estado:
    ;mov edi, SCREENESTADO       ;cambio la pantalla a estado.
    push SCREENESTADO
    call cambiar_pantalla
    call load_pantalla
    pop edi
    jmp fin_teclado
print_number:
    ;breakpoint  
    sub cl, 1              ;si es un numero entonces siempre es uno menos al que está ahi. entocnes si apreté la tecla 1 su scan code es el 0x02 entonces menos 1 me da lo que necesito
    ;rdrand ax               ;esta instruccion guarda un numero random en el registro del tamaño del registro.
    or al, 0x07             ;seteo los primeros 3 bits. Lo hago ya que esa es la letra blanca.
    ;imprimir_texto_mp cl, 0x1, al, 0x0, 0x0          ;Imprimo el numero.

    MOV edi, 0xB8000
    ADD cl, 0x30
    MOV bl, al
    SHL bx, 8
    MOV bl, cl
    MOV [edi], bx
    jmp fin_teclado

print_cero:
    mov cl, 0              ;si es un numero entonces siempre es uno menos al que está ahi. entocnes si apreté la tecla 1 su scan code es el 0x02 entonces menos 1 me da lo que necesito
    ADD cl, 0x30
    ;rdrand ax               ;esta instruccion guarda un numero random en el registro del tamaño del registro.
    or al, 0x07             ;seteo los primeros 3 bits. Lo hago ya que esa es la letra blanca.
    MOV edi, 0xB8000
    MOV bl, al
    SHL bx, 8
    MOV bl, cl
    MOV [edi], bx

fin_teclado:
    call fin_intr_pic1
    popad
    sti
    ret


;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
global int_servicios
int_servicios:
    cli 
    push edx
    call fin_intr_pic1
    call desalojar_tarea
    cmp eax, ANCLA
    je .SYSTEM_ANCLA
    cmp eax, MISIL
    je .SYSTEM_MISIL
    cmp eax, NAVEGAR
    je .SYSTEM_NAVEGAR

    .SYSTEM_ANCLA:
        mov eax, cr3
        mov ecx, TASK_ANCLA
        push ebx
        push eax
        push ecx
        call mmu_mapear_pagina
        jmp .fin

    .SYSTEM_MISIL:
        mov eax, cr3
        push eax
        push ecx
        push ebx
        call canionear
    .SYSTEM_NAVEGAR:
        mov eax, cr3
        push ecx
        push ebx
        push eax
        call navegar
.fin: 
    call saltar_idle
    pop edx
    sti
    ret

;;
;;Rutina de atencion de interrupcion de bandera
;;--------------------------------------------------------------------------- ;;
global int_bandera
int_bandera:
    cli 
    pushad
    call fin_intr_pic1
    mov eax, 0x42
    call saltar_idle
    popad
    sti
    ret


;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
proximo_reloj:
    pushad

    inc DWORD [reloj_numero]
    mov ebx, [reloj]
    cmp ebx, 0x4
    jl .ok
        mov DWORD [reloj_numero], 0x0
        mov ebx, 0
    .ok:
        add ebx, reloj
        imprimir_texto_mp ebx, 1, 0x0f, 24, 79

    popad
    ret
