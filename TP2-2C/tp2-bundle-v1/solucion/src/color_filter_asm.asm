global color_filter_asm

section .data
mask_b: DB 	0x00 0x00 0x00 0x00
			0x00 0x00 0x00 0x00
			0x00 0x00 0x0f 0x0c
			0x09 0x06 0x03 0x00

mask_g: DB 	0x00 0x00 0x00 0x00
			0x00 0x00 0x00 0x00
			0x00 0x00 0x00 0x0D
			0x0a 0x07 0x04 0x01

mask_r: DB 	0x00 0x00 0x00 0x00
			0x00 0x00 0x00 0x00
			0x00 0x00 0x00 0x0e
			0x0b 0x08 0x05 0x02

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
		MOVUPS xmm0, [rdi + anchoPixels * contadorFilas + contadorColumnas]
		PXOR xmm10, xmm10 					;|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|
		MOVUPS xmm1, xmm0 			 					
		MOVUPS xmm2, xmm0
		MOVUPS xmm3, xmm0

		PSHUFB xmm1, [mask_b]				;|.|.|.|.|.|.|.|.|.|.|b|b|b|b|b|b|
		PSHUFB xmm2, [mask_g]				;|.|.|.|.|.|.|.|.|.|.|.|g|g|g|g|g|
		PSHUFB xmm3, [mask_r]				;|.|.|.|.|.|.|.|.|.|.|.|r|r|r|r|r|





		;PUNPCKLBW xmm0, xmm10				;|0|a7|0|a6|0|a5|0|a4|0|a3|0|a2|0|a1|0|a0|
		;PUNPCKHBW xmm1, xmm10				;|0|a15|0|a14|0|a13|0|a12|0|a11|0|a10|0|a9|0|a8|
		

	.finFila

	POP rbp
    ret


;int distancia(unsigned char rc, unsigned char bc, unsigned char gc, int r, int b, int g)
distancia:
	;PUSH
