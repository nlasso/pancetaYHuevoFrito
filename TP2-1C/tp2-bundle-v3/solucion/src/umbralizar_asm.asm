; void umbralizar_c (
; 	unsigned char *src,
; 	unsigned char *dst,
; 	int h,
; 	int w,
; 	int row_size,
; 	unsigned char min,
; 	unsigned char max,
; 	unsigned char q
; );

; Parámetros:
; 	rdi = src
; 	rsi = dst
; 	rdx = h
; 	rcx = w
; 	r8 = row_size
; 	r9 = min
; 	rbp + 16 = max
; 	rbp + 24 = q


global umbralizar_asm

section .text

umbralizar_asm:
	push rbp
	mov rbp, rsp
	push r14
	push r15
	push rbx

	PXOR xmm0, xmm0 ; xmm0 = 0..0
	PXOR xmm1, xmm1 ; xmm1 = 0..0
	PXOR xmm2, xmm2 ; xmm2 = 0..0
	PXOR xmm3, xmm3 ; xmm3 = 0..0
	PXOR xmm6, xmm6 ; xmm6 = 0..0 , q float 4 veces constante
	PXOR xmm7, xmm7 ; xmm7 = 0..0 , 0 constante

	MOVZX ebx, byte [rbp + 24] ; Q
	MOVD xmm6, ebx
	SHUFPS xmm6, xmm6, 0h ; Q en int 4 veces
	CVTDQ2PS xmm6, xmm6 ; Q en float 4 veces

	sub r8, rcx ; diferencia ancho

	; Resto de w/16
	mov rax, rcx
	mov r10, rdx ; Para no perder h
	mov edx, eax
	shr edx, 16 ; shift derecha, parte alta de edx en parte baja
	mov r11, rcx ; Para no perder w
	mov cx, 16
	idiv cx
	; El resto queda en DX
	movsx edx, dx ; extiendo el signo
	movsx r15, edx ; El resto queda en r15

	mov r14, r15 ; r14 como bool checkear Alineamiento

	mov rdx, r10 ; Devuelvo h
	mov rcx, r11 ; Devuelvo w

	MOVZX ebx, byte [rbp + 16] ; max

	;-------------------------------

.cicloAltura:

	mov r10, rcx ; Para recorrer el ancho de la imagen (w) cada vez

.cicloAncho:

	MOVDQU xmm0, [rdi] ; 16 pixeles a procesar

.highHighPix:
	MOVDQA xmm1, xmm0
	PUNPCKHBW xmm1, xmm7 ; 8 high int
	PUNPCKHWD xmm1, xmm7 ; 4 high high int
	CVTDQ2PS xmm1, xmm1 ; 4 high high float

	PXOR xmm4, xmm4
	PXOR xmm5, xmm5
	MOVDQA xmm4, xmm1
	MOVQ xmm5, r9 ; min
	SHUFPS xmm5, xmm5, 0h ; min en int 4 veces
	CVTDQ2PS xmm5, xmm5 ; min en float 4 veces
	CMPNLTPS xmm4, xmm5 ; xmm4 = 0 si es menor a min, 1 (0xFFFFFFFF) si no lo es (cada doble word)
	ANDPS xmm1, xmm4

	MOVDQA xmm4, xmm1
	MOVD xmm5, ebx
	SHUFPS xmm5, xmm5, 0h ; max en int 4 veces
	CVTDQ2PS xmm5, xmm5 ; max en float 4 veces
	CMPNLEPS xmm4, xmm5 ; xmm4 = 1 (0xFFFFFFFF) si es mayor a max, 0 si no lo es (cada doble word)

	DIVPS xmm1, xmm6 ; Divido por Q

	CVTTPS2DQ xmm1, xmm1 ; xmm1 high high int truncados
	CVTDQ2PS xmm1, xmm1 ; xmm1 high high float
	MULPS xmm1, xmm6 ; Multiplico por Q
	CVTTPS2DQ xmm1, xmm1 ; Convierto a int
	ORPS xmm1, xmm4 ; Con esto, los mayores a max quedan en 0xFFFFFFFF

.highLowPix:
	MOVDQA xmm2, xmm0
	PUNPCKHBW xmm2, xmm7 ; 8 high int
	PUNPCKLWD xmm2, xmm7 ; 4 high low int
	CVTDQ2PS xmm2, xmm2 ; 4 high low float

	PXOR xmm4, xmm4
	PXOR xmm5, xmm5
	MOVDQA xmm4, xmm2
	MOVQ xmm5, r9 ; min
	SHUFPS xmm5, xmm5, 0h ; min en int 4 veces
	CVTDQ2PS xmm5, xmm5 ; min en float 4 veces
	CMPNLTPS xmm4, xmm5 ; xmm4 = 0 si es menor a min, 1 (0xFFFFFFFF) si no lo es (cada doble word)
	ANDPS xmm2, xmm4

	MOVDQA xmm4, xmm2
	MOVD xmm5, ebx
	SHUFPS xmm5, xmm5, 0h ; max en int 4 veces
	CVTDQ2PS xmm5, xmm5 ; max en float 4 veces
	CMPNLEPS xmm4, xmm5 ; xmm4 = 1 (0xFFFFFFFF) si es mayor a max, 0 si no lo es (cada doble word)

	DIVPS xmm2, xmm6 ; Divido por Q

	CVTTPS2DQ xmm2, xmm2 ; xmm2 high low int truncados
	CVTDQ2PS xmm2, xmm2 ; xmm2 high low float
	MULPS xmm2, xmm6 ; Multiplico por Q
	CVTTPS2DQ xmm2, xmm2 ; Convierto a int
	ORPS xmm2, xmm4 ; Con esto, los mayores a max quedan en 0xFFFFFFFF

	;Hago todos positivos poniendo el primer bit en 0
	MOV rax, 7FFFFFFF7FFFFFFFh
	MOVQ xmm4, rax;7FFFFFFF7FFFFFFFh
	MOVLHPS XMM4, XMM4

	ANDPS xmm2, xmm4
	ANDPS xmm1, xmm4
	; Ahora son todos positivos y los puedo empaquetar :-)

	;Empaqueto en xmm2
	packssdw xmm2, xmm1

	;-------------------------------

.lowHighPix:
	MOVDQA xmm1, xmm0
	PUNPCKLBW xmm1, xmm7 ; 8 low int
	PUNPCKHWD xmm1, xmm7 ; 4 low high int
	CVTDQ2PS xmm1, xmm1 ; 4 low high float

	PXOR xmm4, xmm4
	PXOR xmm5, xmm5
	MOVDQA xmm4, xmm1
	MOVQ xmm5, r9 ; min
	SHUFPS xmm5, xmm5, 0h ; min en int 4 veces
	CVTDQ2PS xmm5, xmm5 ; min en float 4 veces
	CMPNLTPS xmm4, xmm5 ; xmm4 = 0 si es menor a min, 1 (0xFFFFFFFF) si no lo es (cada doble word)
	ANDPS xmm1, xmm4

	MOVDQA xmm4, xmm1
	MOVD xmm5, ebx
	SHUFPS xmm5, xmm5, 0h ; max en int 4 veces
	CVTDQ2PS xmm5, xmm5 ; max en float 4 veces
	CMPNLEPS xmm4, xmm5 ; xmm4 = 1 (0xFFFFFFFF) si es mayor a max, 0 si no lo es (cada doble word)

	DIVPS xmm1, xmm6 ; Divido por Q

	CVTTPS2DQ xmm1, xmm1 ; xmm1 low high int truncados
	CVTDQ2PS xmm1, xmm1 ; xmm1 low high float
	MULPS xmm1, xmm6 ; Multiplico por Q
	CVTTPS2DQ xmm1, xmm1  ; Convierto a int
	ORPS xmm1, xmm4 ; Con esto, los mayores a max quedan en 0xFFFFFFFF


.lowLowPix:
	MOVDQA xmm3, xmm0
	PUNPCKLBW xmm3, xmm7 ; 8 low int
	PUNPCKLWD xmm3, xmm7 ; 4 low low int
	CVTDQ2PS xmm3, xmm3 ; 4 low low float

	PXOR xmm4, xmm4
	PXOR xmm5, xmm5
	MOVDQA xmm4, xmm3
	MOVQ xmm5, r9 ; min
	SHUFPS xmm5, xmm5, 0h ; min en int 4 veces
	CVTDQ2PS xmm5, xmm5 ; min en float 4 veces
	CMPNLTPS xmm4, xmm5 ; xmm4 = 0 si es menor a min, 1 (0xFFFFFFFF) si no lo es (cada doble word)
	ANDPS xmm3, xmm4

	MOVDQA xmm4, xmm3
	MOVD xmm5, ebx
	SHUFPS xmm5, xmm5, 0h ; max en int 4 veces
	CVTDQ2PS xmm5, xmm5 ; max en float 4 veces
	CMPNLEPS xmm4, xmm5 ; xmm4 = 1 (0xFFFFFFFF) si es mayor a max, 0 si no lo es (cada doble word)

	DIVPS xmm3, xmm6 ; Divido por Q

	CVTTPS2DQ xmm3, xmm3 ; xmm3 low low int truncados
	CVTDQ2PS xmm3, xmm3 ; xmm3 low low float
	MULPS xmm3, xmm6 ; Multiplico por Q
	CVTTPS2DQ xmm3, xmm3 ; Convierto a int
	ORPS xmm3, xmm4 ; Con esto, los mayores a max quedan en 0xFFFFFFFF

	;Hago todos positivos poniendo el primer bit en 0
	MOV rax, 7FFFFFFF7FFFFFFFh
	MOVQ xmm4, rax;7FFFFFFF7FFFFFFFh
	MOVLHPS XMM4, XMM4

	ANDPS xmm3, xmm4
	ANDPS xmm1, xmm4
	; Ahora son todos positivos y los puedo empaquetar :-)

	; Empaqueto en xmm3
	packssdw xmm3, xmm1

	;-------------------------------

	;Empaqueto xmm2 y xmm3
	PACKUSWB xmm3, xmm2 ; Acá es donde necesitaba que sean todos positivos!!

	;enviarlo a la nueva imagen
	MOVDQU [rsi], xmm3 ;

	; Checkear Alineamiento
	cmp r14, 0
	je .noAlinear

	add rdi, r15
	add rsi, r15
	sub r10, r15

	mov r14, 0 ; Para no volver a alinear la fila
	jmp .cicloAncho

.noAlinear:

	;avanzar a los próximos 16 pixeles
	add rdi, 16
	add rsi, 16
	sub r10, 16

	cmp r10, 0
	jne .cicloAncho
	;Fin cicloAncho

	mov r14, r15 ; Para alinear la nueva fila

	add rdi, r8 ; Me salteo los pixeles que no se ven y no deben ser procesados
	add rsi, r8 ; Me salteo los pixeles que no se ven y no deben ser procesados
	dec rdx

	cmp rdx, 0
	jne .cicloAltura

	;Fin cicloAltura

.fin:
	pop rbx
	pop r15
	pop r14
	pop rbp
	ret
