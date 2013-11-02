ORG 0x1200

BITS 16

    cli
    xchg bx,bx ;breakpoint

;---------------------- ESCRIBIMOS UN MENSAJE EN MODO REAL -------------------;
 
    jmp pre_loop_print;

 texto:	db "modo real";

 tam_text: dw $-texto

 formato: ;0000001 = 0x01
	  ;0 - no blink;000 fondo negro
	  ;0 no brillo ;001 verde	
	  db 0x01

;segmento B800 (dir 0xb8000) es imprimir

pre_loop_print: ;es fs gs
   mov cx, [tam_text]
   mov ax, 0xb800
   mov es, ax ; ES = VIDEO
   mov ax, 0x0
   mov fs, ax; FS = TEXTO
   mov bx, 0x0

loop_print: 
   mov al, [fs:texto]
   mov ah, [fs:formato]

   ;mov al, [fs:texto+bx]
   ;mov ah, [fs:formato]
   ;mov [es:bx]

   xchg bx,bx ;breakpoint
   ;mov WORD [es:0x0], 0x0
   ;mov WORD [es:0x2], 0x0
   ;mov WORD [es:0x4], 0x0
   ;mov WORD [es:0x6], 0x0
   ;mov WORD [es:0x8], 0x0
   mov [es:0x0], ax
   
   ;loop loop_print;   

   jmp $ ;FINAL IMPRIMIR ESTE MODO

; completar

;--------------------------------- CARGAMOS GDT ------------------------------;
   lgdt [gdt_desc]
;------------------------------- ACTIVAMOS BIT PE ----------------------------;
   mov eax, cr0
   or eax, 1
   mov cr0, eax
   ;jmp [0x8:mp]   

BITS 32
;mp:

;----------------- ESCRIBIMOS UN MENSAJE EN MODO PROTEGIDO -------------------;

; completar gdt
;----------------- ACA TERMINA NUESTRO CODIGO 'LO COLGAMOS' ------------------;
    jmp $


;=================== A PARTIR DE ESTE PUNTO PEGO LA GDT ======================;

gdt:
	db 0x0000000000000000 ; INUTIL PRIMER GDT

	;segment code nivel 0
	sc0: dd 0x80003FFF
	     dd 0x00409A00	
	;segment data nivel 0
	sd0: dd 0x00003FFF
	     dd 0x00009200
	;video
	video: dd 0x80000FBF
	     dd 0x0040920B
	gdt_desc: dw $-gdt ;size
		  dd gdt   ; doct
