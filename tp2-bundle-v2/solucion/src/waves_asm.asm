; void waves_c (
;	unsigned char *src,
;	unsigned char *dst,
;	int h,
;	int w,
;	int row_size,
;	float x_scale,
;	float y_scale,
;	float g_scale
; );

; Parámetros:
; 	rdi = src
; 	rsi = dst
; 	edx = h
; 	ecx = w
; 	r8d = row_size
; 	xmm0 = x_scale
; 	xmm1 = y_scale
; 	xmm2 = g_scale

; Toma un float repetido 4 veces en xmm0, lo divide por 8, calcula el sin_taylor y lo multiplica por el scale.
; Pone el resultado en xmm0 repetido 4 veces
; Pisa xmm0, xmm1, xmm2 y xmm3
%macro scale_sin_taylor_div_8 1
  ; xmm0(W) = j | j | j | j
  divps xmm0, xmm9 ; Divido por 8
  movaps xmm1, xmm0 ; Lo copio a xmm1
  ; xmm1(W) = x | x | x | x
  divps xmm0, xmm10
  divps xmm0, xmm11 ; Divido por 2PI
  cvttps2dq xmm0, xmm0 ; Paso a int truncando
  cvtdq2ps xmm0, xmm0 ; Vuelvo a float
  ; xmm0(W) = k | k | k | k
  mulps xmm0, xmm10
  mulps xmm0, xmm11 ; Multiplico por 2PI
  subps xmm1, xmm0 ; xmm1 - xmm0
  ; xmm1(W) = r | r | r | r
  subps xmm1, xmm11 ; Resto PI
  ; xmm1(W) = x | x | x | x
  movaps xmm0, xmm1 ; Lo copio a xmm0. Acá quedará el resultado final
  ; xmm0(W) = x | x | x | x
  mulps xmm1, xmm1; Lo multiplico por sí mismo
  ; xmm1(W) = x² | x² | x² | x²
  movaps xmm2, xmm0 ; xmm2 = xmm0
  ; xmm2(W) = x | x | x | x
  movaps xmm3, xmm2 ; xmm3 = xmm2. Este es tmp
  ; xmm3(W) = x | x | x | x
  mulps xmm3, xmm1
  ; xmm3(W) = x³ | x³ | x³ | x³
  divps xmm3, xmm13
  ; xmm3(W) = x³/6 | x³/6 | x³/6 | x³/6
  subps xmm0, xmm3
  ; xmm0(W) = x-x³/6 | x-x³/6 | x-x³/6 | x-x³/6
  movaps xmm3, xmm2
  ; xmm3(W) = x | x | x | x
  mulps xmm3, xmm1
  ; xmm3(W) = x³ | x³ | x³ | x³
  mulps xmm3, xmm1
  ; xmm3(W) = x⁵ | x⁵ | x⁵ | x⁵
  divps xmm3, xmm14
  ; xmm3(W) = x⁵/120 | x⁵/120 | x⁵/120 | x⁵/120
  addps xmm0, xmm3
  ; xmm0(W) = x-x³/6+x⁵/120 | x-x³/6+x⁵/120 | x-x³/6+x⁵/120 | x-x³/6+x⁵/120
  movaps xmm3, xmm2
  ; xmm3(W) = x | x | x | x
  mulps xmm3, xmm1
  ; xmm3(W) = x³ | x³ | x³ | x³
  mulps xmm3, xmm1
  ; xmm3(W) = x⁵ | x⁵ | x⁵ | x⁵
  mulps xmm3, xmm1
  ; xmm3(W) = x⁷ | x⁷ | x⁷ | x⁷
  divps xmm3, xmm15
  ; xmm3(W) = x⁷/5040 | x⁷/5040 | x⁷/5040 | x⁷/5040
  subps xmm0, xmm3
  ; xmm0(W) = x-x³/6+x⁵/120-x⁷/5040 | x-x³/6+x⁵/120-x⁷/5040 | x-x³/6+x⁵/120-x⁷/5040 | x-x³/6+x⁵/120-x⁷/5040
  mulps xmm0, %1 ; Multiplico por el scale
  ; xmm0(W) = scale * sin_taylor | scale * sin_taylor | scale * sin_taylor | scale * sin_taylor
%endmacro

; Toma un entero, lo divide por 8, calcula el sin_taylor y lo multiplica por el scale.
; Pone el resultado en xmm0 repetido 4 veces
; Pisa xmm0, xmm1, xmm2 y xmm3
%macro scale_sin_taylor_div_8_int 2
  movq xmm0, %1
  shufps xmm0, xmm0, 0 ; Lo repito 4 veces
  ; xmm0(W) = j | j | j | j
  cvtdq2ps xmm0, xmm0 ; Paso a float
  scale_sin_taylor_div_8 %2
%endmacro

; Toma el valor de i y la ubicacion del precalculado sin_taylor(j/8)*y_scale para
; calcular el prof(i, j) para 4 pixels a partir de i
; Devuelve el resultado en xmm0
; Pisa xmm0, xmm1, xmm2 y xmm3
%macro prof_i_j 2
  mov rax, %1
	add rax, 3 ; Pongo en rax el valor del índice para poner los distintos i en un registro
	movq xmm0, rax
	pslldq xmm0, 4
	dec rax
	movq xmm1, rax
	por xmm0, xmm1
	pslldq xmm0, 4
	dec rax
	movq xmm1, rax
	por xmm0, xmm1
	pslldq xmm0, 4
	dec rax
	movq xmm1, rax
	por xmm0, xmm1
	; xmm0 = i | i+1 | i+2 | i+3
	cvtdq2ps xmm0, xmm0 ; Convierto a float
	
	; Calculo el sin_taylor
	scale_sin_taylor_div_8 xmm6
	; xmm0(W) = x_scale*sin_taylor(i/8) | x_scale*sin_taylor((i+1)/8) | x_scale*sin_taylor((i+2)/8) | x_scale*sin_taylor((i+3)/8)
	; Sumo el sin_taylor de j y divido por 2 para conseguir prof(i,j)
	addps xmm0, %2
	divps xmm0, xmm10
	; xmm0(W) = prof(i,j) | prof(i+1,j) | prof(i+2,j) | prof(i+3,j)
	; Multiplico por el g_scale
	mulps xmm0, xmm8
	; xmm0(W) = prof(i,j) * g_scale | prof(i+1,j) * g_scale | prof(i+2,j) * g_scale | prof(i+3,j) * g_scale
%endmacro

; Toma una lista de 16 pixels en xmm12 y la procesa de a 4
%macro process_pixels 0
  ; Unpack y proceso de a 4
  mov rcx, r9
	%rep 4
	  pxor xmm1, xmm1
	  pxor xmm4, xmm4
	  punpcklbw xmm1, xmm12
	  ; xmm1(W) = p0 | p1 | p2 | p3 | p4 | p5 | p6 | p7
	  punpcklwd xmm4, xmm1
	  psrldq xmm4, 3
	  ; xmm4(DW) = p0 | p1 | p2 | p3
	  ; Convierto a float
	  cvtdq2ps xmm4, xmm4
	  prof_i_j rcx, xmm5
	  ; xmm0(W) = prof(i,j) * g_scale | prof(i+1,j) * g_scale | prof(i+2,j) * g_scale | prof(i+3,j) * g_scale
	  addps xmm0, xmm4 ; Sumo al valor del pixel
	  cvtps2dq xmm0, xmm0; Convierto a int
	  packusdw xmm0, xmm0 ; Pack con saturación a word
	  packuswb xmm0, xmm0 ; Pack con saturación a byte
	
	  ; Limpio los lugares con basura para hacer un POR
	  psrldq xmm12, 4
	  pslldq xmm12, 4
	  ; xmm12(B) = 0 | 0 | 0 | 0 | p4 | p5 | p6 | p7 | p8 | p9 | p10 | p11 | p12 | p13 | p14 | p15
	  pslldq xmm0, 12
	  psrldq xmm0, 12
	  ; xmm0(B) = f(p0) | f(p1) | f(p2) | f(p3) | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
	  por xmm12, xmm0
	  ; xmm12(B) = f(p0) | f(p1) | f(p2) | f(p3) | p4 | p5 | p6 | p7 | p8 | p9 | p10 | p11 | p12 | p13 | p14 | p15
	  ; Roto de a 4
	  shufps xmm12, xmm12, 0x39
	  ; xmm12(B) = p4 | p5 | p6 | p7 | p8 | p9 | p10 | p11 | p12 | p13 | p14 | p15 | f(p0) | f(p1) | f(p2) | f(p3)
	  add rcx, 4 ; Agrego 4 para el prof(i,j)
	%endrep
%endmacro

global waves_asm

section .rodata
  MASK_FLOAT_8: DB 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x41
  MASK_FLOAT_2: DB 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40
  MASK_FLOAT_PI: DB 0xDB, 0x0F, 0x49, 0x40, 0xDB, 0x0F, 0x49, 0x40, 0xDB, 0x0F, 0x49, 0x40, 0xDB, 0x0F, 0x49, 0x40
  MASK_FLOAT_6: DB 0x00, 0x00, 0xC0, 0x40, 0x00, 0x00, 0xC0, 0x40, 0x00, 0x00, 0xC0, 0x40, 0x00, 0x00, 0xC0, 0x40
  MASK_FLOAT_120: DB 0x00, 0x00, 0xF0, 0x42, 0x00, 0x00, 0xF0, 0x42, 0x00, 0x00, 0xF0, 0x42, 0x00, 0x00, 0xF0, 0x42
  MASK_FLOAT_5040: DB 0x00, 0x80, 0x9D, 0x45, 0x00, 0x80, 0x9D, 0x45, 0x00, 0x80, 0x9D, 0x45, 0x00, 0x80, 0x9D, 0x45

section .text

waves_asm:
	push rbp
	mov rbp, rsp
	push r12
	push r13
	push r14
	push r15
	
	; Cargo las máscaras
	movaps xmm6, xmm0
	shufps xmm6, xmm6, 0 ; Mask x_scale
	movaps xmm7, xmm1
	shufps xmm7, xmm7, 0 ; Mask y_scale
	movaps xmm8, xmm2
	shufps xmm8, xmm8, 0 ; Mask g_scale
	movups xmm9, [MASK_FLOAT_8]
	movups xmm10, [MASK_FLOAT_2]
	movups xmm11, [MASK_FLOAT_PI]
	movups xmm13, [MASK_FLOAT_6]
	movups xmm14, [MASK_FLOAT_120]
	movups xmm15, [MASK_FLOAT_5040]
	
	mov r9, 0 ; Offset al pixel
	mov r12, 0 ; Offset a la fila
	; Extiendo para después hacer operaciones más fácil
	movsx r14, edx
	movsx r15, ecx
	movsx r8, r8d
	
	; Obtengo el resto de w / 16 (la cantidad de pixels que proceso por vez)
	mov eax, r15d
  mov edx, eax
  shr edx, 16
  mov cx, 16
  idiv cx
  ; El resto queda en DX
  movsx edx, dx
  movsx r11, edx
  ; R11 = (int) alineacion
  mov r10, r11
  ; R10 = (bool) checkearAlineamiento
  
  ; Calculo el sin_taylor para la altura y lo dejo en xmm5
  scale_sin_taylor_div_8_int r12, xmm7
  movaps xmm5, xmm0
  ; xmm5(W) = sin_taylor(j/8) | sin_taylor(j/8) | sin_taylor(j/8) | sin_taylor(j/8)
	
	.loop:
	
	; Traigo 16 pixels
	movups xmm12, [rdi+r9]
	; xmm12(B) = p0 | p1 | p2 | p3 | p4 | p5 | p6 | p7 | p8 | p9 | p10 | p11 | p12 | p13 | p14 | p15
	
	; Unpack y proceso de a 4
	process_pixels

  ; Escribo en el destino
	movups [rsi+r9], xmm12
	
	; Checkeo si hay que alinear
  cmp r10, 0
  je .noAlinear
  add r9, r11
  mov r10, 0 ; Marco para no alinear de nuevo
  jmp .loop
  
  .noAlinear:
  add r9, 16 ; Avanzo 16 pixels
  cmp r9, r15
  jl .loop ; Si no terminé la fila sigo
  
  mov r10, r11 ; Marco para ver si hay que alinear la próxima fila
  inc r12 ; Avanzo 1 fila
  ; Recalculo el sin_taylor para la altura y lo dejo en xmm5
  scale_sin_taylor_div_8_int r12, xmm7
  movaps xmm5, xmm0
  ; xmm5(W) = sin_taylor(j/8) | sin_taylor(j/8) | sin_taylor(j/8) | sin_taylor(j/8)
  mov r9, 0 ; Reseteo el offset al pixel
  add rdi, r8 ; Avanzo 1 fila
  add rsi, r8 ; Avanzo 1 fila
  cmp r12, r14
  jl .loop ; Si quedan filas sigo

  pop r15
  pop r14
  pop r13
  pop r12
  pop rbp
	ret
