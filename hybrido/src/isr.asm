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
extern print_error_code
;; MMU
extern mmu_mapear_pagina
extern canionear
extern navegar
extern anclar

;;SCHED
extern restar_quantum
extern dame_tarea_actual
extern desalojar_tarea
extern desalojar_tarea_actual
extern saltar_idle
extern clock
extern bandera
extern saltar_idle
extern esta_en_flag
extern servicio_en_flag

;;SCREEN ERROR RELATED
extern print_error
extern print_tablaerror
extern estado_error
extern print_bandera
extern print_banderines
extern print_tablatar_int_actual


;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
    jmp GDT_CODE_0:.fix_segments; ME ASEGURO DE TENER UN CS VALIDO
.fix_segments: 
    cli  
    push eax
    mov eax, 0xa0
    mov ds, eax
    ;; Aca solo hay 2 segmentos que me importan, ds y ss.
    ;; Si ss es el segmento corrompido, guardo ax en una posicion de memoria
    ;; Si ds es el segmento corrompido, pusheo ax

    pop eax
    mov [estado_error], eax ; EAX
    mov eax, 0xAB;; al menos que haya habido un errorcode, son todas 90
    mov [estado_error+52], eax; DS

    mov eax, %1 
    push eax
    call print_error_code
    cmp eax, 1
    pop eax
    jne .no_errorcode
    pop eax
    shr eax, 3
    mov [estado_error+52], eax; DS
.no_errorcode:    
    pop eax
    mov [estado_error+32], eax;; eip
    pop eax
    mov [estado_error+48], eax; CS
    pop eax
    mov [estado_error+72], eax; eflags
    
    
    mov eax, ebx
    mov [estado_error+4], eax
    mov eax, ecx
    mov [estado_error+8], eax
    mov eax, edx    
    mov [estado_error+12], eax
    mov eax, esi
    mov [estado_error+16], eax
    mov eax, edi
    mov [estado_error+20], eax
    mov eax, ebp
    mov [estado_error+24], eax
    mov eax, esp
    mov [estado_error+28], eax
    mov eax, cr0
    mov [estado_error+36], eax
    mov eax, cr2
    mov [estado_error+40], eax
    mov eax, cr3
    mov [estado_error+44], eax
    mov eax, es
    mov [estado_error+56], eax
    mov eax, fs
    mov [estado_error+60], eax
    mov eax, gs
    mov [estado_error+64], eax
    mov eax, ss    ;mov eax, ss
    mov [estado_error+68], eax
    


    call print_tablaerror;
    mov eax, %1
    push eax
    call print_tablatar_int_actual
    call print_error
    pop  eax
    CALL load_pantalla;
    ;breakpoint
    call desalojar_tarea_actual
    call saltar_idle
    sti
    ; To Infinity And Beyond!!
    jmp $
    iret
%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler
reloj_numero:           dd 0x00000000
reloj:                  db '|/-\'
numeros_msj:            db '1234567890'
numeros_len equ         $ - numeros_msj
offset:                 dd 0
selector:               dw 0

;;
;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;

global jump_idle
jump_idle:
    pushad
    mov ax, GDT_IDLE
    mov [selector], ax
    jmp far [offset]
    popad
    ret


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
    CALL clock 
    cmp eax, 0
    je .fin
    mov [selector], ax
    ;breakpoint

    ;; NOTA: LOS POPAD Y LOS STI SE DEBEN A QUE LAS BANDERAS
    ;; NO VAN A VOLVER A LA INTERUPCION DE RELOJ, LES RESETEAMOS EL
    ;; EIP. ES NECESARIO ENTONCES TENER TODOS LOS VALORES CARGADOS
    ;; NUEVAMENTE. ESTO INCLUYE A LAS INTERRUPCIONES
    popad;;     VER NOTA
    sti;;       VER NOTA
    ;breakpoint
    jmp far [offset] ;; REVISAR ESTO
    cli;;       VER NOTA
    pushad;;    VER NOTA
.fin
    popad
    sti
    iret

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
    iret


;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
global int_servicios
int_servicios:
    cli 
    pushad
    push eax
    call esta_en_flag
    cmp eax, 1
    je .ERROR
    pop eax
    ;breakpoint
    cmp eax, ANCLA
    je .SYSTEM_ANCLA
    cmp eax, MISIL
    je .SYSTEM_MISIL
    cmp eax, NAVEGAR
    je .SYSTEM_NAVEGAR
    jmp .fin
    .SYSTEM_ANCLA:
        push ebx
        call anclar        
        pop ebx
        jmp .fin
    .SYSTEM_MISIL:
        push ecx
        push ebx
        call canionear
        pop ebx
        pop ecx
        jmp .fin
    .SYSTEM_NAVEGAR:
        push ecx
        push ebx
        call navegar
        pop ecx
        pop ebx
        jmp .fin
    .ERROR:
        call servicio_en_flag
        pop eax

.fin: 
    push edx
    call fin_intr_pic1
    call saltar_idle
    pop edx
    popad
    sti
    iret

;;
;;Rutina de atencion de interrupcion de bandera
;;--------------------------------------------------------------------------- ;;
global int_bandera
int_bandera:
    cli 
    push ecx
    push ebx
    push edx
    push eax
    call bandera
    pop eax
    call fin_intr_pic1
    pop edx
    pop ebx
    pop ecx
    sti
    iret
