%macro get_miniature_color 5
%endmacro
global miniature_asm

section .data align = 16
	MASK_ORD: DB 0x00, 0x03, 0x06, 0x09, 0x0C, 0x01, 0x04, 0x07, 0x0A, 0x0D, 0x02, 0x05, 0x08, 0x0B, 0x0E, 0x80
	MASK_ORD1: DB 0x00, 0x03, 0x06, 0x09, 0x0C, 0x01, 0x04, 0x07, 0x0A, 0x0D, 0x02, 0x05, 0x08, 0x0B, 0x0E, 0x0f
	WHITE: DB 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff


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
					MOVDQU xmm4, [WHITE]
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
					MOVDQU xmm4, [WHITE]
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
