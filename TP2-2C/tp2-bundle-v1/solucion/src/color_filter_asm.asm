global color_filter_asm

section .rodata align = 16
	MASK_REORD: DB 	0x00, 0x03, 0x06, 0x09, 0x0C, 0x80, 0x80, 0x80,	0x80, 0x80, 0x80, 0x80,	0x80, 0x80, 0x80, 0x80

	MASK_UNPCKL: DB 0x00, 0x80, 0x80, 0x80,	0x01, 0x80, 0x80, 0x80,	0x02, 0x80, 0x80, 0x80,	0x03, 0x80, 0x80, 0x80

	MASK_UNPCKH: DB 0x04, 0x80, 0x80, 0x80,	0x80, 0x80, 0x80, 0x80,	0x80, 0x80, 0x80, 0x80,	0x80, 0x80, 0x80, 0x80

	MASK_REARM: DB 	0x00, 0x00, 0x00, 0x04,	0x04, 0x04, 0x08, 0x08,	0x08, 0x0C, 0x0C, 0x0C,	0x80, 0x80, 0x80, 0x80

	MASK_REARM1: DB	0x80, 0x80, 0x80, 0x80,	0x80, 0x80, 0x80, 0x80,	0x80, 0x80, 0x80, 0x80,	0x00, 0x00, 0x00, 0x80

	CERO: DB 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	MASK_TRES: DB 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00

	UNO: DB		0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01

	MASK_EFE: DD 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
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

;src = rdi
;dst = rsi
;rc = dl; es un byte nomas
;gc = cl es un byte nomas
;bc = r8b byte
;threshold = r9d dword
;width = [esp]??
;height = [esp + 4]??


color_filter_asm:
	; xmm(B/W) = interpretado como Byte/Word, la posición MENOS significativa a la IZQUIERDA (como el gdb)
	PUSH rbp
	MOV rbp, rsp
	PUSH r15
	PUSH r14
	PUSH r13
	PUSH r12
	PUSH rbx


	MOV r13, rdx			;r13 = rc
	MOV r12, rcx 		;r12 = gc
	MOV rbx, r8 		;rbx = bc
	MOV ecx, [rbp + 16] ;width en r15d
	MOV r14d, [rbp + 24] ;height en 

	;primero proceso la cantidad de filas y columnas para los loops.
	;Necesito multiplicar el ancho por tres para tener la cantidad de bytes total por fila.
	MOV eax, 3
	MUL ecx
	MOV ecx, eax ;width * 3
	MOV eax, r14d ;eax = height
	MUL ecx
	MOV ecx, eax	;ecx = width * height * 3

	; Cargo las máscaras
  	PXOR xmm9, xmm9 ; Todo en 0
  	; xmm3(W) = 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0

  	MOVD xmm1, r13d
  	PSHUFB xmm1, [CERO]
  	;xmm1(W) = | rc | rc | rc | rc | rc | rc | rc | rc | rc | rc | rc | rc | rc | rc | rc | rc |
  	
  	MOVD xmm2, r12d
  	PSHUFB xmm2, [CERO]
  	;xmm2(W) = | gc | gc | gc | gc | gc | gc | gc | gc | gc | gc | gc | gc | gc | gc | gc | gc |

  	MOVD xmm3, ebx
  	PSHUFB xmm3, [CERO]
  	;xmm4(W) = | bc | bc | bc | bc | bc | bc | bc | bc | bc | bc | bc | bc | bc | bc | bc | bc |

	.ciclo:
		CMP rcx, 0
		JLE .fin
		MOVDQU xmm0, [rdi] 	;xmm0(B) = | b | g | r | b | g | r | b | g | r | b | g | r | b | g | r | b |
		;jmp .final
		MOVDQA xmm4, xmm0 	
		MOVDQA xmm5, xmm0 	
		MOVDQA xmm6, xmm0	;Me lo guardo por las dudas.

		PSHUFB xmm0, [MASK_REORD] 		;xmm0(B) = | b | b | b | b | b | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
		MOVDQA xmm7, xmm0 				;xmm7 = xmm0
		MOVDQA xmm15, [MASK_REORD]
		PADDB xmm15, [UNO]
		PSHUFB xmm4, xmm15				;xmm4(B) = | g | g | g | g | g | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
		MOVDQA xmm8, xmm4 				;xmm8 = xmm4
		PADDB xmm15, [UNO]
		PSHUFB xmm5, xmm15				;xmm5(B) = | r | r | r | r | r | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
		MOVDQA xmm10, xmm5 				;xmm10 = xmm5

		;Para hacer la diferencia absoluta, obtengo el minimo y el maximo y luego los resto. De esta forma
		;nunca voy a tener un numero negativo.
		PMINUB xmm7, xmm3
		PMAXUB xmm0, xmm3
		PSUBB xmm0, xmm7		;xmm0(B) = | b - bc | b - bc | b - bc | b - bc | b - bc | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |

		PMINUB xmm8, xmm2
		PMAXUB xmm4, xmm2
		PSUBB xmm4, xmm8		;xmm4(B) = | g - gc | g - gc | g - gc | g - gc | g - gc | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |

		PMINUB xmm10, xmm1
		PMAXUB xmm5, xmm1
		PSUBB xmm5, xmm10		;xmm5(B) = | r - rc | r - rc | r - rc | r - rc | r - rc | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |

		;Limpio los registros temporales
		PXOR xmm8, xmm8
		PXOR xmm7, xmm7
		PXOR xmm10, xmm10

		;Copio las restas para desempaquetar.
		MOVDQA xmm7, xmm0
		MOVDQA xmm8, xmm4
		MOVDQA xmm10, xmm5

		;Desempaqueto de byte a float.
		PSHUFB xmm0, [MASK_UNPCKL]
		PSHUFB xmm7, [MASK_UNPCKH]

		PSHUFB xmm4, [MASK_UNPCKL]
		PSHUFB xmm8, [MASK_UNPCKH]

		PSHUFB xmm5, [MASK_UNPCKL]
		PSHUFB xmm10, [MASK_UNPCKH]

		;Convierto de int a float
		CVTDQ2PS xmm0, xmm0
		CVTDQ2PS xmm7, xmm7
		CVTDQ2PS xmm4, xmm4
		CVTDQ2PS xmm8, xmm8
		CVTDQ2PS xmm5, xmm5
		CVTDQ2PS xmm10, xmm10

		;Los multiplico po si mismos o sea al cuadrado.
		MULPS xmm0, xmm0
		MULPS xmm7, xmm7
		MULPS xmm4, xmm4
		MULPS xmm8, xmm8
		MULPS xmm5, xmm5
		MULPS xmm10, xmm10

		;Sumo por pixel.
		ADDPS xmm0, xmm4
		ADDPS xmm0, xmm5

		ADDPS xmm7, xmm8
		ADDPS xmm7, xmm10

		;Finalmente hago la raiz.
		SQRTPS xmm0, xmm0
		SQRTPS xmm7, xmm7

		PXOR xmm5, xmm5

	  	MOVD xmm5, r9d
	  	PSHUFD xmm5, xmm5, 0	;| threshold | threshold | threshold | threshold | 

	  	CVTPS2DQ xmm0, xmm0
	  	CVTPS2DQ xmm7, xmm7

	  	PCMPGTD xmm0, xmm5
	  	PCMPGTD xmm7, xmm5

	  	;Me armo la mascara para poder ver quien suma y quien queda como esta.

	  	PSHUFB xmm0, [MASK_REARM]
	  	PSHUFB xmm7, [MASK_REARM1]

	  	;Los sumo, en realidad es para que me quede la mascara con todos. pero al final algunos estan en 0
	  	;y otros estan con el valor de la mascara entonces al final me queda la mascara completa con los 4 valores.
	  	PADDB xmm0, xmm7
	  	;Ahora puedo usar el xmm6 que es el que tenia guardado por las dudas para evitarme mas accesos a memoria.
	  	MOVDQA xmm4, xmm6 ;Me lo vuelvo a guardar.

	  	;Ahora me quedan solo los que voy a querer operar.
	  	MOVDQA xmm12, xmm0
	  	PAND xmm4, xmm0
	  	;Y en xmm6 me quedan los que quiero que queden iguales el resto en 0.
	  	PXOR xmm12, [MASK_EFE]
	  	PAND xmm6, xmm12

	  	MOVDQA xmm7, xmm4
	  	MOVDQA xmm8, xmm4

	  	;Para hacer la suma de los b + g + r / 3, primero lso tengo que desempaquetar ya que es probable que la suma de 
	  	;esos tres pase los 255 y necesite un dato de tamano word. Para esto primero shifteo todo y me quedan los rgb 
	  	;alineados.
	  	PSHUFB xmm4, [MASK_REORD]
	  	MOVDQA xmm15, [MASK_REORD]
	  	PADDB xmm15, [UNO]
	  	PSHUFB xmm7, xmm15
	  	PADDB xmm15, [UNO]
	  	PSHUFB xmm8, xmm15

	  	;Luego me hago las copias y lo desempaqueto.
	  	MOVDQA xmm11, xmm4
	  	MOVDQA xmm12, xmm7
	  	MOVDQA xmm13, xmm8

	  	PSHUFB xmm4, [MASK_UNPCKL]
	  	PSHUFB xmm11, [MASK_UNPCKH]

	  	PSHUFB xmm7, [MASK_UNPCKL]
	  	PSHUFB xmm12, [MASK_UNPCKH]

	  	PSHUFB xmm8, [MASK_UNPCKL]
	  	PSHUFB xmm13, [MASK_UNPCKH]

;
	  	;PADDB xmm4, xmm7
	  	;PADDB xmm4, xmm8
;
	  	;MOVDQA xmm10, xmm4
	  	;PSHUFB xmm4, [MASK_UNPCKL]
	  	;PSHUFB xmm10, [MASK_UNPCKH]

	  	;Hago una mascara de 3,con tamaño de datos DoubleWord.
	  	MOVDQA xmm14, [MASK_TRES]

	  	;Convierto todo a Float para poder operar en la division.
	  	CVTDQ2PS xmm14, xmm14
	  	CVTDQ2PS xmm4, xmm4
	  	CVTDQ2PS xmm11, xmm11
	  	CVTDQ2PS xmm7, xmm7
	  	CVTDQ2PS xmm12, xmm12
	  	CVTDQ2PS xmm8, xmm8
	  	CVTDQ2PS xmm13, xmm13

	  	ADDPS xmm4, xmm7
	  	ADDPS xmm4, xmm8

	  	ADDPS xmm11, xmm12
	  	ADDPS xmm11, xmm13

	  	DIVPS xmm4, xmm14
	  	DIVPS xmm11, xmm14

	  	;Lo devuelvo a int para despues empaquetarlo. Se que lso bits que sean 0 quedaron asi.
	  	CVTPS2DQ xmm4, xmm4
	  	CVTPS2DQ xmm11, xmm11

	  	PSHUFB xmm4, [MASK_REARM]
	  	PSHUFB xmm11, [MASK_REARM1]

	  	PADDB xmm4, xmm11

	  	PADDB xmm6, xmm4
;.final:
	  	MOVDQU [rsi], xmm6

	  	ADD rdi, 15
	  	ADD rsi, 15
	  	SUB rcx, 15
	  	JMP .ciclo

.fin:
	POP rbx
	POP r12
	POP r13
	POP r14
	POP r15
	POP rbp
	RET
