
%define breakpoint xchg bx, bx


;==============================================================================
;------------------------------------------------------------------------------
ORG 0x1200
BITS 16
    cli
    breakpoint
    jmp msgr
;------------------------------------------------------------------------------
;==============================================================================



;==============================================================================
;---------------------- ESCRIBIMOS UN MENSAJE EN MODO REAL -------------------;
%define formatoR	00001001b
;					brgbbrgb
msgreal:	dB	"|Estoy en modo real. Programar asi es una cagada ! Quiero mis 8 o 16 registros de proposito general, es horrible tener solo 4 y no poder usarlos para cualquier cosa buuuuuuuu...|"
lmsgr EQU $ - msgreal
;------------------------------------------------------------------------------
msgr: breakpoint
	mov AX, 0xb800
	mov ES, AX
		
	xor CX, CX
	xor DX, DX

	mov Ah, formatoR
ciclomsgr:
	mov BX, CX
	mov Al, [CS:msgreal+BX]

	mov BX, DX
	mov word [ES:BX], AX

	inc DX
	inc DX
	inc CX

	cmp CX, lmsgr
	jne ciclomsgr

	jmp cargoGDT
;------------------------------------------------------------------------------
;==============================================================================



;==============================================================================
;--------------------------------- CARGAMOS GDT ------------------------------;
cargoGDT:	breakpoint
	cli
	lgdt [gdt_desc]
	mov EAX, CR0
	or EAX, 1
	mov CR0, EAX
	jmp 0x8:mp
;==============================================================================



;==============================================================================
;------------------------------- ACTIVAMOS BIT PE ----------------------------;
BITS 32									;Cargo selectores de segmento
mp:	breakpoint
	xor EAX, EAX

	mov AX, 10000b
	mov DS, AX
	mov ES, AX
	mov GS, AX

	mov AX, 11000b
	mov FS, AX
	jmp 0X8:msgp
;==============================================================================



;==============================================================================
;----------------- ESCRIBIMOS UN MENSAJE EN MODO PROTEGIDO -------------------;
%define formatoP 	00000100b
;					brgbbrgb
msgprot:	dB	"|Estoy en modo Protegido. A pesar de tener que usar los descriptores de segmento la vida esta siendo mas feliz|"
lmsgp EQU $ - msgprot
;------------------------------------------------------------------------------
msgp:breakpoint
	mov EDX, lmsgr
	shl EDX, 1
	xor ECX, ECX

	mov Ah, formatoP
ciclomsgp:
	mov EBX, ECX
	mov Al, [msgprot+EBX]

	mov EBX, EDX
	mov word [FS:EBX], AX

	inc EDX
	inc EDX
	inc ECX

	cmp ECX, lmsgp
	jne ciclomsgp

	jmp fin
;------------------------------------------------------------------------------
;==============================================================================



;==============================================================================
;----------------- ACA TERMINA NUESTRO CODIGO 'LO COLGAMOS' ------------------;
fin:	breakpoint 
	jmp fin
;------------------------------------------------------------------------------
;==============================================================================



;==============================================================================
;=================== A PARTIR DE ESTE PUNTO PEGO LA GDT ======================;
;------------------------------------------------------------------------------
gdt:	dD 0x00000000					;GDT.0 	= 	NULL
		dD 0x00000000
;---------------------------------------
sc0:	dD 0x00003FFF 					;GDT.1 	=	Código nivel 0
		dD 0x00C09A00
;---------------------------------------
sc1:	dD 0x00003FFF 					;GDT.1 	=	Datos nivel 0
		dD 0x00C09200
;---------------------------------------
video:	dD 0x80000FBF 					;GDT.2	=	video (datos nivel 0)
		dD 0x0040920B
;---------------------------------------
gdt_desc:	dW $ - gdt 
			dD gdt
;------------------------------------------------------------------------------
;==============================================================================
