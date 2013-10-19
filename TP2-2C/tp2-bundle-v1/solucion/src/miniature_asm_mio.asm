; Esta macro en %1 tiene una fila b | b | b | b | b | g | g | g | g | g | r | r | r | r | r | 0
; en %2 tiene la fila correspondiente a M, para poder multiplicarlo por la fila del primer parametro
; en %3 tiene la parte de M que no alcanzo para que este en un solo parametro
; en %4 tiene a xmm10, que es el que lleva la suma de los 3 colores
%macro sum_row 4
; --------------- cuenta primer color: b --------------
movdqu xmm8, %1 ; copio %1 a xmm8
pshufb xmm8, [MASK_FIRST_5B] ; xmm8 = b | b | b | b | b | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0

;desempaqueto %1 los 5 b, de byte a word
punpckhbw xmm8, [CERO] ; xmm8 =  0 b | 0 b | 0 b | 0 b | 0 b | 0 0 | 0 0 | 0 0
movdqu xmm9, xmm8 ; xmm9 = 0 b | 0 b | 0 b | 0 b | 0 b | 0 0 | 0 0 | 0 0

;desempaqueto a dword, me queda b en 2 registros
punpckhwd xmm8, [CERO] ; xmm8 = 0 0 0 b | 0 0 0 b | 0 0 0 b | 0 0 0 b
punpcklwd xmm9, [CERO] ; xmm9 = 0 0 0 b | 0 0 0 0 | 0 0 0 0 | 0 0 0 0

; multiplico por 1 fila de la matriz M
pmuludq xmm8, %2 ; xmm8 * M0 
pmuludq xmm9, %3 ; xmm9 * M01

;Convierto de int a float, para hacer la suma horizontal
cvtdq2ps xmm8, xmm8
cvtdq2ps xmm9, xmm9

; suma horizontal xmm8
haddps xmm8, xmm8 ; xmm8 = xmm8[2] + xmm8[3] | xmm8[0] + xmm8[1] | xmm8[2] + xmm8[3] | xmm8[0] + xmm8[1]
haddps xmm8, [CERO] ; xmm8 = xmm8[2] + xmm8[3] + xmm8[0] + xmm8[1] | lo mismo | 0 0 0 0 | 0 0 0 0

; suma vertical con xmm9
addps xmm8, xmm9 ; xmm8 = xmm8[2] + xmm8[3] + xmm8[0] + xmm8[1] + xmm9[0] | suma | 0 0 0 0 | 0 0 0 0 
pshufd xmm8, [MASK_FIRST_DW] ; xmm8 = 0 | suma color g | 0 | 0

addps %4, xmm8 ; xmm10 = suma total b | suma total g | suma total r | 0

; --------------- cuenta segundo color: g --------------
movdqu xmm8, %1 ; copio %1 a xmm8
pshufb xmm8, [MASK_SECOND_5B] ; xmm8 = g | g | g | g | g | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0

;desempaqueto %1 los 5 b, de byte a word
punpckhbw xmm8, [CERO] ; xmm8 =  0 g | 0 g | 0 g | 0 g | 0 g | 0 0 | 0 0 | 0 0
movdqu xmm9, xmm8 ; xmm9 = 0 g | 0 g | 0 g | 0 g | 0 g | 0 0 | 0 0 | 0 0

;desempaqueto a dword, me queda b en 2 registros
punpckhwd xmm8, [CERO] ; xmm8 = 0 0 0 g | 0 0 0 g | 0 0 0 g | 0 0 0 g
punpcklwd xmm9, [CERO] ; xmm9 = 0 0 0 g | 0 0 0 0 | 0 0 0 0 | 0 0 0 0

; multiplico por 1 fila de la matriz M
pmuludq xmm8, %2 ; xmm8 * M1
pmuludq xmm9, %3 ; xmm9 * M11

;Convierto de int a float, para hacer la suma horizontal
cvtdq2ps xmm8, xmm8
cvtdq2ps xmm9, xmm9

; suma horizontal xmm8
haddps xmm8, xmm8 ; xmm8 = xmm8[2] + xmm8[3] | xmm8[0] + xmm8[1] | xmm8[2] + xmm8[3] | xmm8[0] + xmm8[1]
haddps xmm8, [CERO] ; xmm8 = xmm8[2] + xmm8[3] + xmm8[0] + xmm8[1] | lo mismo | 0 0 0 0 | 0 0 0 0

; suma vertical con xmm9
addps xmm8, xmm9 ; xmm8 = xmm8[2] + xmm8[3] + xmm8[0] + xmm8[1] + xmm9[0] | suma | 0 0 0 0 | 0 0 0 0 
pshufd xmm8, [MASK_SECOND_DW] ; xmm8 = 0 | suma de color g | 0 | 0

addps %4, xmm8 ; xmm10 = suma total b | suma total g | suma total r | 0

; --------------- cuenta segundo color: r --------------
movdqu xmm8, %1 ; copio %1 a xmm8
pshufb xmm8, [MASK_THIRD_5B] ; xmm8 = g | g | g | g | g | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0

;desempaqueto %1 los 5 b, de byte a word
punpckhbw xmm8, [CERO] ; xmm8 =  0 g | 0 g | 0 g | 0 g | 0 g | 0 0 | 0 0 | 0 0
movdqu xmm9, xmm8 ; xmm9 = 0 g | 0 g | 0 g | 0 g | 0 g | 0 0 | 0 0 | 0 0

;desempaqueto a dword, me queda b en 2 registros
punpckhwd xmm8, [CERO] ; xmm8 = 0 0 0 g | 0 0 0 g | 0 0 0 g | 0 0 0 g
punpcklwd xmm9, [CERO] ; xmm9 = 0 0 0 g | 0 0 0 0 | 0 0 0 0 | 0 0 0 0

; multiplico por 1 fila de la matriz M
pmuludq xmm8, %2 ; xmm8 * M1
pmuludq xmm9, %3 ; xmm9 * M11

;Convierto de int a float, para hacer la suma horizontal
cvtdq2ps xmm8, xmm8
cvtdq2ps xmm9, xmm9

; suma horizontal xmm8
haddps xmm8, xmm8 ; xmm8 = xmm8[2] + xmm8[3] | xmm8[0] + xmm8[1] | xmm8[2] + xmm8[3] | xmm8[0] + xmm8[1]
haddps xmm8, [CERO] ; xmm8 = xmm8[2] + xmm8[3] + xmm8[0] + xmm8[1] | lo mismo | 0 0 0 0 | 0 0 0 0

; suma vertical con xmm9
addps xmm8, xmm9 ; xmm8 = xmm8[2] + xmm8[3] + xmm8[0] + xmm8[1] + xmm9[0] | suma | 0 0 0 0 | 0 0 0 0 
pshufd xmm8, [MASK_THIRD_DW] ; xmm8 = 0 | suma de color g | 0 | 0

addps %4, xmm8 ; xmm10 = suma total b | suma total g | suma total r | 0

%endmacro

; Esta macro hace la cuenta de la subimagen de 5 x 5 pixels, por la matriz M
; %1 %2 %3 %4 %5 son 5 pixels cada uno, correspondiente a una fila distinta, y separados por colores
; por ejemplo %1 vale -> b | b | b | b | b | g | g | g | g | g | r | r | r | r | r | 0
; y cambia en %3 el pixel del medio, es decir el b del medio, el g del medio y el r del medio, 
; 	por el resultado de la cuenta
%macro get_miniature_color 5

pxor xmm10, xmm10 ; xmm10 = 0, llevara la suma de los productos
				  ; xmm10 deberia quedar => suma color b | suma color g | suma color r | 0 0 0 0

; proceso por fila, hago el producto por la matriz M, y guardo la suma en xmm10
sum_row %1, [M0], [M01], xmm10
sum_row %2, [M1], [M11], xmm10
sum_row %3, [M2], [M21], xmm10
sum_row %4, [M1], [M11], xmm10
sum_row %5, [M0], [M01], xmm10
; xmm10 queda en float => suma color b | suma color g | suma color r | 0 0 0 0

movdqu xmm8, [DIV_600] ; xmm8 = 600 | 600 | 600 | 0
cvtdq2ps xmm8, xmm8 ; convierto xmm8 a float para dividir
divps xmm10, xmm8 ; xmm10 = suma color b / 600| suma color g / 600 | suma color r / 600 | 0 0 0 0

; Ahora tengo que empaquetar a byte
; convierto de nuevo a int
cvtps2dq xmm10, xmm10 ; convierto xmm10 a int para empaquetar
pxor xmm11, xmm11 ; xmm11 = 0

; xmm10 = 000b | 000g | 000r |0000
; quiero lograr =>
; 0b | 0g | 0r | 00 | 00 | 00 | 00 | 00 
packusdw xmm11, xmm10 ; xmm11 =  0b | 0g | 0r | 00 | 00 | 00 | 00 | 00 

pxor xmm10, xmm10 ; xmm10 = 0
packuswb xmm10, xmm11 ; xmm10 =  b | g | r | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0

; Ahora tengo que colocar el b g r que tengo en xmm10 en el parametro %3
; %3 vale -> b | b | b | b | b | g | g | g | g | g | r | r | r | r | r | 0

; acomodo xmm10 para eso
pshufb xmm10, [MASK_OVERRIDE] ; xmm10 = 0 | 0 | b | 0 | 0 | 0 | 0 | g | 0 | 0 | 0 | 0 | r | 0| 0 | 0
pshufb %3, [MASK_OVERRIDE_INVERSE] ; pone en 0 donde tengo que sobreescribir

paddb %3, xmm10

%endmacro

global miniature_asm

section .data align = 16
	MASK_ORD: DB 0x00, 0x03, 0x06, 0x09, 0x0C, 0x01, 0x04, 0x07, 0x0A, 0x0D, 0x02, 0x05, 0x08, 0x0B, 0x0E, 0x80
	MASK_ORD1: DB 0x00, 0x03, 0x06, 0x09, 0x0C, 0x01, 0x04, 0x07, 0x0A, 0x0D, 0x02, 0x05, 0x08, 0x0B, 0x0E, 0x0f

	M0: DD 1, 5, 18, 5
	M01: DD 1, 0, 0, 0

	M1: DD 5, 32, 64, 32
	M11: DD 5, 0, 0, 0

	M2: DD 18, 64, 100, 64
	M21: DD 18, 0, 0, 0

	MASK_FIRST_5B: DB 0x00, 0x01, 0x02, 0x03, 0x04, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
	MASK_SECOND_5B: DB 0x05, 0x06, 0x07, 0x08, 0x09, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
	MASK_THIRD_5B: DB 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80

	MASK_FIRST_DW: DD 0x0000, 0x8000, 0x8000, 0x8000
	MASK_SECOND_DW: DD 0x8000, 0x0000, 0x8000, 0x8000
	MASK_THIRD_DW: DD 0x8000, 0x8000, 0x0000, 0x8000

	MASK_OVERRIDE: DB 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x80, 0x01, 0x80, 0x80, 0x80, 0x80, 0x02, 0x80, 0x80, 0x80
	MASK_OVERRIDE_INVERSE: DB 0x00, 0x01, 0x80, 0x03, 0x04, 0x05, 0x06, 0x80, 0x08, 0x09, 0x0A, 0x0B, 0x80, 0x0D, 0x0E, 0x0F

	DIV_600: DD 600, 600, 600, 1
	CERO: DB 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
section .text


; void miniature_asm(unsigned char *src,
;                unsigned char *dst,
;                int width,
;                int height,
;                float topPlane,
;                float bottomPlane,
;                int iters); 
;*src = rdi
;*dst = rsi
;Width = edx
;Height = ecx
;xmm0 = topPlane
;xmm1 = bottomPlane
;r8d = iters
miniature_asm:
	PUSH rbp
	MOV rbp, rsp
	PUSH r15
	PUSH r14
	PUSH r13
	PUSH r12
	PUSH rbx

	MOV r15d, edx	;Width
	MOV r14d, ecx	;Height

	;Muevo Height a un xmm para poder calcular el alto de las bandas.
	MOVD xmm2, r14d
	CVTDQ2PS xmm2, xmm2
	MULPS xmm0, xmm2	;topPlaneLimit
	MULPS xmm1, xmm2	;bottomPlaneInit
	;(Width - 2) * 3
	SUB r15d, 2
	MOV eax, 3
	MUL r15d
	MOV r15d, eax		;newWidth

	SUB r14d, 2 		;Le quito 2 filas al alto para no pasarme al final.

	;Ya no uso mas xmm2
	PXOR xmm2, xmm2

	;Pongo mi contador en 0
	XOR r10, r10
.bandaAlta:
	CMP r10d, r8d			;i (contador) < iters
	JGE .bandaIntermedia
	;Hago todo el procesamiento para calcular el alto de la banda en esta iteracion. Esto se repite por iteracion ya que va
	;cambiando como se puede apreciar a continuacion.
	MOVD xmm3, r8d			;xmm3(D) = iters | 0 | 0 | 0 |
	MOVD xmm4, r10d			;xmm4(D) = i (contador) | 0 | 0 | 0 |
	CVTDQ2PS xmm3, xmm3		;xmm3 -> Float
	CVTDQ2PS xmm4, xmm4		;xmm4 -> Float
	MOVDQU xmm5, xmm4
	MULPS xmm5, xmm0		;xmm5 = topPlaneLimit * i | 0 | 0 | 0 |
	DIVPS xmm5, xmm3		;xmm5 = topPlaneLimit * i /iters | 0 | 0 | 0 |
	SUBPS xmm5, xmm0		;xmm5 (topPlaneLimit) = topPlaneLimit - xmm5
	CVTPS2DQ xmm5, xmm5
	XOR r9, r9 			;r9 = 0
	MOVD r9d, xmm5		;r9 = topPlaneLimit - ((topPlaneLimit * i) / iters)

	;limpio todo lo que no uso y seteo en 0 a los contadores que voy a usar proximamente.
	PXOR xmm3, xmm3
	PXOR xmm4, xmm4
	XOR r13, r13		;r13 = 0
	XOR r12, r12		;r12 = 0
		.filasBandaAlta:
			CMP r13d, r9d		;mientras fila < topPlaneLimit - ((topPlaneLimit * i) / iters)
			JG .finBandaAlta
				.columnasBandaAlta:
					CMP r12d, r15d
					JG .finColumnasAlta
					MOV ebx, r15d	;newWidth
					ADD ebx, 6		;Le sumo los dos pixels que saqué ya que no los proceso pero los necesito para moverme.
					MOV rax, r13
					MUL rbx 		; ancho en bytes * contador de filas.
					MOV rbx, rax
					ADD rbx, rdi
					ADD rbx, r12
					MOVDQU xmm2, [rbx]
					ADD rbx, r15
					MOVDQU xmm3, [rbx]
					ADD rbx, r15
					MOVDQU xmm4, [rbx]
					ADD rbx, r15
					MOVDQU xmm6, [rbx]
					ADD rbx, r15
					MOVDQU xmm7, [rbx]
;
					;PSHUFB xmm2, [MASK_ORD]
					;PSHUFB xmm3, [MASK_ORD]
					;PSHUFB xmm4, [MASK_ORD1]
					;PSHUFB xmm6, [MASK_ORD]
					;PSHUFB xmm7, [MASK_ORD]

					;get_miniature_color xmm2, xmm3, xmm4, xmm6, xmm7

					SUB rbx, r15
					SUB rbx, r15
					MOVDQU [rbx], xmm4
					SUB rbx, rdi
					ADD rbx, rsi
					MOVDQU [rbx], xmm4
					ADD r12d, 3
					XOR rbx, rbx
					JMP .columnasBandaAlta
					.finColumnasAlta:
					ADD r13d, 1
					XOR r12, r12
					JMP .filasBandaAlta
				.finBandaAlta:
				XOR r13, r13
				XOR r12, r12
				ADD r10d, 1
				JMP .bandaAlta

.bandaIntermedia:
	XOR r10, r10
	XOR r12, r12
	MOVDQU xmm2, xmm1
	CVTPS2DQ xmm2, xmm2
	MOVD r10d, xmm2
	.filasBandaIntermedia:
	CMP r13d, r10d
	JGE .startBottom
	.columnasBandaIntermedia:
		CMP r12d, r15d
		JG .finColumnasIntermedias

		;------------------------TODO: revisar si se puede hacer mas eficiente--------------

		MOV rbx, r15
		ADD rbx, 6
		MOV rax, r13
		MUL rbx
		MOV rbx, rax
		ADD rbx, r12
		MOVDQU xmm2, [rdi + rbx]
		MOVDQU [rsi + rbx], xmm2
		XOR rbx, rbx
		ADD r12d, 16
		JMP .columnasBandaIntermedia
		.finColumnasIntermedias:
		ADD r13d, 1
		XOR r12, r12
		JMP .filasBandaIntermedia

.startBottom:
	XOR r10, r10

.iteracionBottom:
	CMP r10d, r8d			;i (contador) < iters
	JGE .fin
	;Hago todo el procesamiento para calcular el alto de la banda en esta iteracion. Esto se repite por iteracion ya que va
	;cambiando como se puede apreciar a continuacion.
	MOVD xmm3, r8d			;xmm3(D) = iters | 0 | 0 | 0 |
	MOVD xmm4, r10d			;xmm4(D) = i (contador) | 0 | 0 | 0 |
	MOVD xmm6, r14d			;xmm6(D) = Height | 0 | 0 | 0 |
	CVTDQ2PS xmm6, xmm6
	MOVDQU xmm7, xmm1
	SUBPS xmm7, xmm6		;xmm7(F) = Height - bottomPlaneInit
	CVTDQ2PS xmm3, xmm3		;xmm3 -> Float
	CVTDQ2PS xmm4, xmm4		;xmm4 -> Float
	MULPS xmm4, xmm7		;xmm4 = (Height - bottomPlaneInit) * i | 0 | 0 | 0 |
	DIVPS xmm4, xmm3		;xmm4 = (Height - bottomPlaneInit) * i /iters | 0 | 0 | 0 |
	ADDPS xmm4, xmm1		;xmm4 (bottomPlaneInit) = bottomPlaneInit + xmm4
	CVTPS2DQ xmm4, xmm4
	XOR r9, r9 			;r9 = 0
	MOVD r9d, xmm4		;r9 = bottomPlaneInit + (((Height - bottomPlaneInit) * i) / iters)

	;limpio todo lo que no uso y seteo en 0 a los contadores que voy a usar proximamente.
	PXOR xmm3, xmm3
	PXOR xmm4, xmm4
	PXOR xmm7, xmm7
	PXOR xmm6, xmm6
	XOR r13, r13		;r13 = 0
	XOR r12, r12		;r12 = 0
		.filasBandaBaja:
			CMP r9d, r14d		;mientras height > bottomPlaneInit - ((bottomPlaneInit * i) / iters)
			JG .finBandaBaja
				.columnasBandaBaja:
					CMP r12d, r15d
					JG .finColumnasBaja
					MOV ebx, r15d	;newWidth
					ADD ebx, 6		;Le sumo los dos pixels que saqué ya que no los proceso pero los necesito para moverme.
					MOV rax, r9
					MUL rbx 		; ancho en bytes * contador de filas.
					MOV rbx, rax
					ADD rbx, rdi
					ADD rbx, r12
					MOVDQU xmm2, [rbx]
					ADD rbx, r15
					MOVDQU xmm3, [rbx]
					ADD rbx, r15
					MOVDQU xmm4, [rbx]
					ADD rbx, r15
					MOVDQU xmm6, [rbx]
					ADD rbx, r15
					MOVDQU xmm7, [rbx]

					;PSHUFB xmm2, [MASK_ORD]
					;PSHUFB xmm3, [MASK_ORD]
					;PSHUFB xmm4, [MASK_ORD1]
					;PSHUFB xmm6, [MASK_ORD]
					;PSHUFB xmm7, [MASK_ORD]

					;get_miniature_color xmm2, xmm3, xmm4, xmm6, xmm7

					SUB rbx, r15
					SUB rbx, r15
					MOVDQU [rbx], xmm4
					SUB rbx, rdi
					ADD rbx, rsi
					MOVDQU [rbx], xmm4
					ADD r12d, 3
					XOR rbx, rbx
					JMP .columnasBandaBaja
					.finColumnasBaja:
					ADD r9d, 1
					XOR r12, r12
					JMP .filasBandaBaja
				.finBandaBaja:
				XOR r9, r9
				XOR r12, r12
				ADD r10d, 1
				JMP .iteracionBottom

.fin:
	POP rbx
	POP r12
	POP r13
	POP r14
	POP r15
	POP rbp
    RET
