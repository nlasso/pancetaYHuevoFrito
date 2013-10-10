ORG 0x1200


BITS 16
JMP start

msj_modo_real: DB "Estoy en modo real..."
len_modo_real: DW $ - msj_modo_real
msj_modo_protegido: DB 'Estoy en modo protegido...'
len_modo_protegido: DW $ - msj_modo_protegido

gdt:	dd 0x0 
		dd 0x0 

sc0: dd 0x00003FFF
	 dd 0x00C09A00

sd0: dd 0x00003FFF
	 dd 0x00C09200

video: 	dd 0x80000FBF
	   	dd 0x0040920B

gdt_desc:	dw $-gdt
			dd gdt

start:
    cli
    xchg bx,bx ;breakpoint

;---------------------- ESCRIBIMOS UN MENSAJE EN MODO REAL -------------------;

MOV ax, 0xB800		;Memoria de video(comienza en el segmento 0xB8000)
MOV es, ax			
MOV bx, 0 			;le paso el largo de mi string
MOV si, 0
.ciclo:
	CMP si, [len_modo_real]				;me muevo hasta el final del string
	JE .next
	MOV byte al, [msj_modo_real + si]		;accedo a un caracter segun el offset cx
	MOV byte [es:bx], al					;Byte de configuracion: blink:7 | fondo: 6,5,4 | brillante: 3 | caracter: 2,1,0
	INC bx
	MOV byte [es:bx], 0x07 					;inserto el caracter.
	INC si								;offset string + 1
	INC bx								;contador + 1
	JMP .ciclo
.next:
	
;--------------------------------- CARGAMOS GDT ------------------------------;
LGDT [gdt_desc]

;------------------------------- ACTIVAMOS BIT PE ----------------------------;

MOV eax, cr0
OR eax, 1
MOV cr0, eax
JMP 0x8:mp

BITS 32

;----------------- ESCRIBIMOS UN MENSAJE EN MODO PROTEGIDO -------------------;
mp:
	XOR eax, eax 
	MOV ax, 10000b
	MOV ds, ax
	MOV es, ax
	MOV gs, ax
	MOV ax, 11000b
	MOV fs, ax
XCHG bx, bx
	;Imprimo en modo protegido.
	MOV eax, 0xB8000
	XOR esi, esi
.cicloMp:
	CMP si, [len_modo_protegido]
	JE .fin
	MOV bl, 0x07
	SHL bx, 8
	MOV bl, [msj_modo_protegido + si]

	MOV [eax], bx
	ADD eax, 2
	INC si
	loop .cicloMp

.fin:
xchg bx,bx
;----------------- ACA TERMINA NUESTRO CODIGO 'LO COLGAMOS' ------------------;
    jmp $


;=================== A PARTIR DE ESTE PUNTO PEGO LA GDT ======================;

; completar