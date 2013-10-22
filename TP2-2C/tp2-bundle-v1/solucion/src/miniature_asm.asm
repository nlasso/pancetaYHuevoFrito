global miniature_asm
; Esta macro en %1 tiene una fila b | b | b | b | b | g | g | g | g | g | r | r | r | r | r | 0
; en %2 tiene la fila correspondiente a M, para poder multiplicarlo por la fila del primer parametro
; en %3 tiene la parte de M que no alcanzo para que este en un solo parametro
; en %4 tiene a xmm10, que es el que lleva la suma de los 3 colores
%macro sum_row 4
; --------------- cuenta primer color: b --------------
movdqu xmm8, %1 ; copio %1 a xmm8
MOVDQU xmm9, %1
pshufb xmm8, [MASK_UNPCKL_5B] ; xmm8 = b | b | b | b | b | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
pshufb xmm9, [MASK_UNPCKH_5B]

MOVDQU xmm12, %2	;xmm12 = M0 primer caso.
MOVDQU xmm13, %3

;;Convierto de int a float, para hacer la suma horizontal
cvtdq2ps xmm8, xmm8
cvtdq2ps xmm9, xmm9
CVTDQ2PS xmm12, xmm12
CVTDQ2PS xmm13, xmm13
;; multiplico por 1 fila de la matriz M
MULPS xmm8, xmm12 ; xmm8 * M0 
MULPS xmm9, xmm13 ; xmm9 * M01

;; suma horizontal xmm8
haddps xmm8, [CERO] ; xmm8 = xmm8[2] + xmm8[3] | xmm8[0] + xmm8[1] | xmm8[2] + xmm8[3] | xmm8[0] + xmm8[1]
haddps xmm8, [CERO] ; xmm8 = xmm8[2] + xmm8[3] + xmm8[0] + xmm8[1] | lo mismo | 0 0 0 0 | 0 0 0 0

;; suma vertical con xmm9
addps xmm8, xmm9 ; xmm8 = xmm8[2] + xmm8[3] + xmm8[0] + xmm8[1] + xmm9[0] | suma | 0 0 0 0 | 0 0 0 0

PXOR xmm11, xmm11
SHUFPS xmm8, xmm11, 0x0C  ; xmm8 = 0 | suma color g | 0 | 0

addps %4, xmm8 ; xmm10 = suma total b | suma total g | suma total r | 0

;; --------------- cuenta segundo color: g --------------
PXOR xmm8, xmm8
movdqu xmm8, %1 ; copio %1 a xmm8
MOVDQU xmm9, %1
pshufb xmm8, [MASK_UNPCKL_5G] ; xmm8 = g | g | g | g | g | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
pshufb xmm9, [MASK_UNPCKH_5G]

;;Convierto de int a float, para hacer la suma horizontal
cvtdq2ps xmm8, xmm8
cvtdq2ps xmm9, xmm9

;; multiplico por 1 fila de la matriz M
MULPS xmm8, xmm12 ; xmm8 * M0 
MULPS xmm9, xmm13 ; xmm9 * M01

;; suma horizontal xmm8
haddps xmm8, [CERO] ; xmm8 = xmm8[2] + xmm8[3] | xmm8[0] + xmm8[1] | xmm8[2] + xmm8[3] | xmm8[0] + xmm8[1]
haddps xmm8, [CERO] ; xmm8 = xmm8[2] + xmm8[3] + xmm8[0] + xmm8[1] | lo mismo | 0 0 0 0 | 0 0 0 0

;; suma vertical con xmm9
addps xmm8, xmm9 ; xmm8 = xmm8[2] + xmm8[3] + xmm8[0] + xmm8[1] + xmm9[0] | suma | 0 0 0 0 | 0 0 0 0

PXOR xmm11, xmm11
SHUFPS xmm8, xmm11, 0x03 ; xmm8 = 0 | suma de color g | 0 | 0

addps %4, xmm8 ; xmm10 = suma total b | suma total g | suma total r | 0

; --------------- cuenta segundo color: r --------------
PXOR xmm8, xmm8
movdqu xmm8, %1 ; copio %1 a xmm8
MOVDQU xmm9, %1
pshufb xmm8, [MASK_UNPCKL_5R] ; xmm8 = g | g | g | g | g | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
pshufb xmm9, [MASK_UNPCKH_5R]
;;Convierto de int a float, para hacer la suma horizontal
cvtdq2ps xmm8, xmm8
cvtdq2ps xmm9, xmm9

;; multiplico por 1 fila de la matriz M
MULPS xmm8, xmm12 ; xmm8 * M0 
MULPS xmm9, xmm13 ; xmm9 * M01

;; suma horizontal xmm8
haddps xmm8, [CERO] ; xmm8 = xmm8[2] + xmm8[3] | xmm8[0] + xmm8[1] | xmm8[2] + xmm8[3] | xmm8[0] + xmm8[1]
haddps xmm8, [CERO] ; xmm8 = xmm8[2] + xmm8[3] + xmm8[0] + xmm8[1] | lo mismo | 0 0 0 0 | 0 0 0 0

;; suma vertical con xmm9
addps xmm8, xmm9 ; xmm8 = xmm8[2] + xmm8[3] + xmm8[0] + xmm8[1] + xmm9[0] | suma | 0 0 0 0 | 0 0 0 0

PXOR xmm11, xmm11
SHUFPS xmm11, xmm8, 0xC0 ; xmm8 = 0 | suma de color g | 0 | 0

addps %4, xmm11 ; xmm10 = suma total b | suma total g | suma total r | 0

%endmacro

; Esta macro hace la cuenta de la subimagen de 5 x 5 pixels, por la matriz M
; %1 %2 %3 %4 %5 son 5 pixels cada uno, correspondiente a una fila distinta, y separados por colores
; por ejemplo %1 vale -> b | b | b | b | b | g | g | g | g | g | r | r | r | r | r | 0
; y cambia en %3 el pixel del medio, es decir el b del medio, el g del medio y el r del medio, 
; 	por el resultado de la cuenta
%macro get_miniature_color 5

pxor xmm10, xmm10 ; xmm10 = 0, llevara la suma de los productos
				  ; xmm10 deberia quedar => suma color b | suma color g | suma color r | 0 0 0 0

MOVDQU xmm11, [M0]
MOVDQU xmm12, [M01]
; proceso por fila, hago el producto por la matriz M, y guardo la suma en xmm10
sum_row %1, xmm11, xmm12, xmm10

MOVDQU xmm11, [M1]
MOVDQU xmm12, [M11]

sum_row %2, xmm11, xmm12, xmm10

MOVDQU xmm11, [M2]
MOVDQU xmm12, [M21]
sum_row %3, xmm11, xmm12, xmm10

MOVDQU xmm11, [M1]
MOVDQU xmm12, [M11]
sum_row %4, xmm11, xmm12, xmm10

MOVDQU xmm11, [M0]
MOVDQU xmm12, [M01]
sum_row %5, xmm11, xmm12, xmm10
; xmm10 queda en float => suma color b | suma color g | suma color r | 0 0 0 0
PXOR xmm8, xmm8
movdqu xmm8, [DIV_600] ; xmm8 = 600 | 600 | 600 | 0
cvtdq2ps xmm8, xmm8 ; convierto xmm8 a float para dividir
divps xmm10, xmm8 ; xmm10 = suma color b / 600| suma color g / 600 | suma color r / 600 | 0 0 0 0

; Ahora tengo que empaquetar a byte
; convierto de nuevo a int
cvtps2dq xmm10, xmm10 ; convierto xmm10 a int para empaquetar
pxor xmm11, xmm11 ; xmm11 = 0

PSHUFB xmm10, [MASK_RECONSTIT]
PSHUFB %3, [MASK_RECONSTIT_ORIG]

PADDB %3, xmm10

%endmacro
section .data 
align 16
	MASK_ORD: DB 0x00, 0x03, 0x06, 0x09, 0x0C, 0x01, 0x04, 0x07, 0x0A, 0x0D, 0x02, 0x05, 0x08, 0x0B, 0x0E, 0x80
	MASK_ORD1: DB 0x00, 0x03, 0x06, 0x09, 0x0C, 0x01, 0x04, 0x07, 0x0A, 0x0D, 0x02, 0x05, 0x08, 0x0B, 0x0E, 0x0f

	M0: DD 0x00000001, 0x00000005, 0x00000012, 0x00000005
	M01: DD 0x000000001, 0x00000000, 0x00000000, 0x00000000

	M1: DD 0x00000005, 0x00000020, 0x00000040, 0x00000020
	M11: DD 0x00000005, 0x00000000, 0x00000000, 0x00000000

	M2: DD 0x00000012, 0x00000040, 0x00000064, 0x00000040
	M21: DD 0x00000012, 0x00000000, 0x00000000, 0x00000000

	DIV_600: DD 0x00000258, 0x00000258, 0x00000258, 0x00000001
	CERO: DB 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	MASK_DOS_PPIO: DD 0x00000002, 0x80000000, 0x80000000, 0x80000000 

	MASK_UNPCKL_5B: DB 0x00, 0x80, 0x80, 0x80, 0x01, 0x80, 0x80, 0x80, 0x02, 0x80, 0x80, 0x80, 0x03, 0x80, 0x80, 0x80
	MASK_UNPCKH_5B: DB 0x04, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80

	MASK_UNPCKL_5G: DB 0x05, 0x80, 0x80, 0x80, 0x06, 0x80, 0x80, 0x80, 0x07, 0x80, 0x80, 0x80, 0x08, 0x80, 0x80, 0x80
	MASK_UNPCKH_5G: DB 0x09, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80

	MASK_UNPCKL_5R: DB 0x0A, 0x80, 0x80, 0x80, 0x0B, 0x80, 0x80, 0x80, 0x0B, 0x80, 0x80, 0x80, 0x0D, 0x80, 0x80, 0x80
	MASK_UNPCKH_5R: DB 0x0E, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80

	MASK_RECONSTIT: DB 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x04, 0x08, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
	MASK_RECONSTIT_ORIG: DB 0x00, 0x05, 0x0A, 0x01, 0x06, 0x0B, 0x80, 0x80, 0x80, 0x03, 0x08, 0x0D, 0x04, 0x09, 0x0E, 0x0F

	MASK_INSERT_SRC: DB 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x06, 0x07, 0x08, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
	MASK_INSERT_DEST: DB 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x80, 0x80, 0x80, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
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
	SUB rsp, 8

	MOV r15d, edx	;Width
	MOV r14d, ecx	;Height

	;Muevo Height a un xmm para poder calcular el alto de las bandas.
	MOVD xmm2, r14d
	CVTDQ2PS xmm2, xmm2
	MULPS xmm0, xmm2	;topPlaneLimit
	MULPS xmm1, xmm2	;bottomPlaneInit
	;(Width - 2) * 3
	;SUB r15d, 6
	MOV eax, 3
	MUL r15d
	SUB eax, 16
	MOV r15d, eax		;newWidth


	SUB r14d, 5 		;Le quito 2 filas al alto para no pasarme al final.

	;Ya no uso mas xmm2
	PXOR xmm2, xmm2

	XOR r10, r10
	XOR r12, r12
	MOVDQU xmm2, xmm1
	CVTPS2DQ xmm2, xmm2
	MOVD r10d, xmm2
	PXOR xmm2, xmm2
	MOVDQU xmm2, [M01]
	CVTDQ2PS xmm2, xmm2
	ADDPS xmm2, xmm0
	CVTPS2DQ xmm2, xmm2
	MOVD r13d, xmm2
	.filasBandaIntermedia:
	CMP r13d, r10d
	JGE .cicloAlto
	.columnasBandaIntermedia:
		CMP r12d, r15d
		JG .finColumnasIntermedias
		MOV rbx, r15
		ADD rbx, 16
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


.cicloAlto:
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
	MOVDQU xmm6, xmm0
	SUBPS xmm6, xmm5		;xmm5 (topPlaneLimit) = topPlaneLimit - xmm5
	CVTPS2DQ xmm6, xmm6
	XOR r9, r9 			;r9 = 0
	MOVD r9d, xmm6		;r9 = topPlaneLimit - ((topPlaneLimit * i) / iters)

	;limpio todo lo que no uso y seteo en 0 a los contadores que voy a usar proximamente.
	PXOR xmm3, xmm3
	PXOR xmm4, xmm4
	PXOR xmm6, xmm6
	PXOR xmm5, xmm5
	XOR r13, r13		;r13 = 0
	XOR r12, r12		;r12 = 0
		.filasBandaAlta:
			CMP r13d, r9d		;mientras fila < topPlaneLimit - ((topPlaneLimit * i) / iters)
			JG .finBandaAlta
				.columnasBandaAlta:
					CMP r12d, r15d
					JG .finColumnasAlta
					MOV ebx, r15d	;newWidth
					ADD ebx, 16		;Le sumo los dos pixels que saqué ya que no los proceso pero los necesito para moverme.
					MOV rax, r13
					MUL rbx 		; ancho en bytes * contador de filas.
					MOV rbx, rax
					ADD rbx, r12
					MOVDQU xmm2, [rdi + rbx]
					ADD rbx, r15
					add rbx, 16
					MOVDQU xmm3, [rdi + rbx]
					ADD rbx, r15
					add rbx, 16
					MOVDQU xmm4, [rdi + rbx]
					ADD rbx, r15
					add rbx, 16
					MOVDQU xmm6, [rdi + rbx]
					ADD rbx, r15
					add rbx, 16
					MOVDQU xmm7, [rdi + rbx]

					PSHUFB xmm2, [MASK_ORD]
					PSHUFB xmm3, [MASK_ORD]
					PSHUFB xmm4, [MASK_ORD1]
					PSHUFB xmm6, [MASK_ORD]
					PSHUFB xmm7, [MASK_ORD]
					get_miniature_color xmm2, xmm3, xmm4, xmm6, xmm7

					SUB rbx, r15
					SUB rbx, 16
					SUB rbx, r15
					SUB rbx, 16
					PXOR xmm7, xmm7
					MOVDQU xmm7, [rsi + rbx]
					PSHUFB xmm7, [MASK_INSERT_DEST]
					PSHUFB xmm4, [MASK_INSERT_SRC]
					PADDB xmm4, xmm7
					MOVDQU [rsi + rbx], xmm4
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
					PXOR xmm6, xmm6
					MOVDQU xmm6, xmm0
					CVTPS2DQ xmm6, xmm6
					MOVD r13d, xmm6
					ADD r12, 2
					.copyFila:
						CMP r12d, r13d
						JG .finCopyFila
						XOR rbx, rbx
						XOR r9, r9
						MOV r9, r15
						ADD r9, 16
						MOV rax, r12
						MUL r9
						MOV r9, rax
						.copyColumna:
							CMP ebx, r15d
							JG .finCopyColumna
							ADD r9, rbx
							MOVDQU xmm4, [rsi + r9]
							MOVDQU [rdi + r9], xmm4
							ADD ebx, 16
							JMP .copyColumna
						.finCopyColumna:
						ADD r12, 1
						JMP .copyFila
				.finCopyFila:
				XOR r12, r12
				XOR r13, r13
				XOR rbx, rbx
				XOR r9, r9
				ADD r10d, 1
				JMP .bandaAlta

.bandaIntermedia:


.startBottom:
	XOR r10, r10
	;SUB r15, 2
.iteracionBottom:
	CMP r10d, r8d			;i (contador) < iters
	JGE .fin
	;;Hago todo el procesamiento para calcular el alto de la banda en esta iteracion. Esto se repite por iteracion ya que va
	;;cambiando como se puede apreciar a continuacion.
	MOVD xmm3, r8d			;xmm3(D) = iters | 0 | 0 | 0 |
	MOVD xmm4, r10d			;xmm4(D) = i (contador) | 0 | 0 | 0 |
	ADD r14d, 5
	MOVD xmm6, r14d			;xmm6(D) = Height | 0 | 0 | 0 |
	SUB r14d, 5
	CVTDQ2PS xmm6, xmm6
	MOVDQU xmm7, xmm1
	SUBPS xmm6, xmm7		;xmm7(F) = Height - bottomPlaneInit
	CVTDQ2PS xmm3, xmm3		;xmm3 -> Float
	CVTDQ2PS xmm4, xmm4		;xmm4 -> Float
	MULPS xmm4, xmm6		;xmm4 = (Height - bottomPlaneInit) * i | 0 | 0 | 0 |
	DIVPS xmm4, xmm3		;xmm4 = (Height - bottomPlaneInit) * i /iters | 0 | 0 | 0 |
	ADDPS xmm4, xmm1		;xmm4 (bottomPlaneInit) = bottomPlaneInit + xmm4
	CVTPS2DQ xmm4, xmm4
	PSUBD xmm4, [MASK_DOS_PPIO]
	XOR r9, r9 			;r9 = 0
	MOVD r9d, xmm4		;r9 = bottomPlaneInit + (((Height - bottomPlaneInit) * i) / iters)

	;limpio todo lo que no uso y seteo en 0 a los contadores que voy a usar proximamente.
	PXOR xmm3, xmm3
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
					ADD ebx, 16		;Le sumo los dos pixels que saqué ya que no los proceso pero los necesito para moverme.
					MOV eax, r9d
					MUL rbx 		; ancho en bytes * contador de filas.
					MOV rbx, rax
					ADD rbx, r12
					MOVDQU xmm2, [rdi + rbx]
					ADD rbx, r15
					ADD rbx, 16
					MOVDQU xmm3, [rdi + rbx]
					ADD rbx, r15
					ADD rbx, 16
					MOVDQU xmm4, [rdi + rbx]
					ADD rbx, r15
					ADD rbx, 16
					MOVDQU xmm6, [rdi + rbx]
					ADD rbx, r15
					ADD rbx, 16
					MOVDQU xmm7, [rdi + rbx]

					PSHUFB xmm2, [MASK_ORD]
					PSHUFB xmm3, [MASK_ORD]
					PSHUFB xmm4, [MASK_ORD1]
					PSHUFB xmm6, [MASK_ORD]
					PSHUFB xmm7, [MASK_ORD]

					get_miniature_color xmm2, xmm3, xmm4, xmm6, xmm7

					SUB rbx, r15
					SUB rbx, 16
					SUB rbx, r15
					SUB rbx, 16
					MOVDQU [rdi + rbx], xmm4
					MOVDQU [rsi + rbx], xmm4
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
	ADD rsp, 8
	POP rbx
	POP r12
	POP r13
	POP r14
	POP r15
	POP rbp
    RET
