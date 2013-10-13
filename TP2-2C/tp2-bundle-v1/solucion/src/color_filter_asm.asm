global color_filter_asm

section .data

section .text
;void color_filter_asm(unsigned char *src,
;                    unsigned char *dst,
;                    unsigned char rc,
;                    unsigned char gc,
;                    unsigned char bc,
;                    int threshold,
;                    int width,
;                    int height);

color_filter_asm:
	PUSH rbp
	MOV rbp, rsp
	PUSH r15
	PUSH r14
	PUSH r13
	PUSH r12
	PUSH rbx

	MOV r15, rdi							;Paso *src = r15
	MOV r14, rsi							;Paso *dst = r14
	XOR r13, r13							;r13 es mi contador de filas
	%define contadorFilas r13
	XOR r12, r12							;r12 es mi contador de columnas
	%define contadorColumnas r12
	MOV rbx, [rsp]							;rbx es el ancho en pixels
	%define anchoPixels rbx
	MOV r10, [rsp + 8]						;r10 es el alto en pixels
	%define altoPixels r10
	SUB altoPixels, 1						;Height - 1
	MOV rax, 3								;Width = Width * 3 (cantidad de pixels)
	MUL anchoPixels
	MOV anchoPixels, rax
	SUB anchoPixels, 3

.loopFilas:
	;while(contadorFilas < cantidadFilas)
	CMP contadorFilas, altoPixels
	JE .procesarRestos
	.loopColumnas:
		CMP contadorColumnas, anchoPixels
		JGE .finFila
		MOVUPS xmm0, [rdi + anchoPixels * contadorFilas + contadorColumnas]		;|b|r|g|b|r|g|b|r|g|b|r|g|b|r|g|b|
		PXOR xmm10, xmm10 														;|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|
		MOVUPS xmm4, xmm0 			 										;xmm1 = xmm0 	|g|b|r|g|b|r|g|b|r|g|b|r|g|b|r|g|b|
		MOVUPS xmm8, xmm1 													;xmm2 = xmm0 	|g|b|r|g|b|r|g|b|r|g|b|r|g|b|r|g|b|

		PSRLDQ xmm4, 1						;0|b|r|g|b|r|g|b|r|g|b|r|g|b|r|g|
		PSRLDQ xmm8, 1						;0|0|b|r|g|b|r|g|b|r|g|b|r|g|b|r|

		;Ahora están todos alineados con el primer byte segun corresponda r,g o b.
		;resto por las constantes en los parametros para calcular la distancia.

		PSUBB xmm0, byte r8					;|g|b - bc|r|g|b - b|r|g|b - bc|r|g|b - bc|r|g|b - bc|r|g|b - bc|
		PSUBB xmm4, cl 						;0|b|r|g - gc|b|r|g - gc|b|r|g - gc|b|r|g - gc|b|r|g - gc|
		PSUBB xmm8, dl 						;0|0|b|r - rc|g|b|r - rc|g|b|r - rc|g|b|r - rc|g|b|r - rc|

		PXOR xmm1, xmm1
		PXOR xmm2, xmm2
		PXOR xmm3, xmm3
		PXOR xmm5, xmm5
		PXOR xmm6, xmm6
		PXOR xmm7, xmm7
		PXOR xmm9, xmm9
		PXOR xmm10, xmm10
		PXOR xmm11, xmm11


		;xmm3 | xmm2 | xmm1 | xmm0
		PUNPCKHBW xmm1, xmm0 				;|g|b|r|g|b|r|g|b|
		PUNPCKLBW xmm2, xmm0 				;|b|r|g|b|r|g|b|r|
		PUNPCKLWD xmm0, xmm1
		PUNPCKHWD xmm1, xmm1
		PUNPCKHWD xmm3, xmm2
		PUNPCKLWD xmm2, xmm2



		;xmm7 | xmm6 | xmm5 | xmm4
		PUNPCKHBW xmm6, xmm4 				;|r|g|b|r|g|b|r|g|
		PUNPCKLBW xmm4, xmm4 				;|0|b|r|g|b|r|g|b|
		PUNPCKHWD xmm5, xmm4
		PUNPCKLWD xmm4, xmm4
		PUNPCKHWD xmm7, xmm6
		PUNPCKLWD xmm6, xmm6

		;xmm11 | xmm10 | xmm9 | xmm8
		PUNPCKHBW xmm10, xmm8 				;|b|r|g|b|r|g|b|r|
		PUNPCKLBW xmm8, xmm8 				;|0|0|b|r|g|b|r|g|
		PUNPCKHWD xmm9, xmm8
		PUNPCKLWD xmm8, xmm8
		PUNPCKHWD xmm11, xmm10
		PUNPCKLWD xmm10, xmm10

		;Los convierto en float para poder hacer la Raiz cuadrada proximamente.
		CVTDQ2PS xmm0, xmm0
		CVTDQ2PS xmm1, xmm1
		CVTDQ2PS xmm2, xmm2
		CVTDQ2PS xmm3, xmm3
		CVTDQ2PS xmm4, xmm4
		CVTDQ2PS xmm5, xmm5
		CVTDQ2PS xmm6, xmm6
		CVTDQ2PS xmm7, xmm7
		CVTDQ2PS xmm8, xmm8
		CVTDQ2PS xmm9, xmm9
		CVTDQ2PS xmm10, xmm10
		CVTDQ2PS xmm11, xmm11

		PSUBD xmm0, byte r8
		PSUBD xmm1, byte r8
		PSUBD xmm2, byte r8
		PSUBD xmm3, byte r8

		PSUBD xmm4, cl
		PSUBD xmm5, cl
		PSUBD xmm6, cl
		PSUBD xmm7, cl

		PSUBD xmm8, dl
		PSUBD xmm9, dl
		PSUBD xmm10, dl
		PSUBD xmm11, dl 

		;Cuadrado.
		MULPS xmm0, xmm0
		MULPS xmm1, xmm1
		MULPS xmm2, xmm2
		MULPS xmm3, xmm3
		MULPS xmm4, xmm4
		MULPS xmm5, xmm5
		MULPS xmm6, xmm6
		MULPS xmm7, xmm7
		MULPS xmm8, xmm8
		MULPS xmm9, xmm9
		MULPS xmm10, xmm10
		MULPS xmm11, xmm11

		;Sumar todos los r, g y b ya que están alineados es 1 a 1
		ADDPS xmm0, xmm4
		ADDPS xmm0, xmm8

		ADDPS xmm1, xmm5
		ADDPS xmm1, xmm9

		ADDPS xmm2, xmm6
		ADDPS xmm2, xmm10

		ADDPS xmm3, xmm7
		ADDPS xmm3, xmm11

		SQRTPS xmm0
		SQRTPS xmm1
		SQRTPS xmm2
		SQRTPS xmm3

		;Los convierto a Ints.
		CVTPS2PI xmm0, xmm0
		CVTPS2PI xmm1, xmm1
		CVTPS2PI xmm2, xmm2
		CVTPS2PI xmm3, xmm3

		;Los empaqueto de Double a Word
		PACKSSDW xmm1, xmm0
		PACKSSDW xmm3, xmm2

		;Los empaqueto de Word a Byte.
		PACKSSWB xmm3, xmm1

		PXOR xmm1, xmm1
		PXOR xmm2, xmm2

		MOVUPS xmm1, xmm3
		MOVUPS xmm2, xmm3

		PCMPGTB xmm3, byte r9
		PAND xmm1, xmm3			;Los que son mayores y hago (r+g+b)/3
		PXOR xmm2, xmm3			;Los que son menores y quedan iguales.

		

	.finFila

	POP rbp
    ret


;int distancia(unsigned char rc, unsigned char bc, unsigned char gc, int r, int b, int g)
distancia:
	;PUSH
