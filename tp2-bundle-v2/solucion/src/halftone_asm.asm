; void halftone_asm (
; 	unsigned char *src,
; 	unsigned char *dst,
; 	int h,
; 	int w,
; 	int src_row_size,
; 	int dst_row_size
; );

; Parámetros:
; 	rdi = src
; 	rsi = dst
; 	edx = h
; 	ecx = w
; 	r8d = src_row_size
; 	r9d = dst_row_size


global halftone_asm

section .rodata
  MASK_PSHUFB: DB 0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
  
section .text
; xmm(B/W) = interpretado como Byte/Word, la posición MENOS significativa a la IZQUIERDA (como el gdb)
; l0p0 = linea 0 pixel 0 (de ese loop)
; b = basura
halftone_asm:
	push rbp
	mov rbp, rsp
	push r12
	push r13
	push r14
	push r15
	
	; Extiendo para después hacer operaciones más fácil
	movsx r14, edx
	movsx r15, ecx
	movsx r8, r8d
	movsx r9, r9d
  
  ; Cargo las máscaras
  pxor xmm3, xmm3 ; Todo en 0
  ; xmm3(W) = 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
  mov r10, 204
  movq xmm4, r10 ; Copio el 204
  shufps xmm4, xmm4, 0 ; Lo extiendo
  ; xmm4(W) = 204 | 0 | 204 | 0 | 204 | 0 | 204 | 0
  mov r10, 409
  movq xmm5, r10 ; Copio el 409
  shufps xmm5, xmm5, 0 ; Lo extiendo
  ; xmm5(W) = 409 | 0 | 409 | 0 | 409 | 0 | 409 | 0
  mov r10, 614
  movq xmm6, r10 ; Copio el 614
  shufps xmm6, xmm6, 0 ; Lo extiendo
  ; xmm6(W) = 614 | 0 | 614 | 0 | 614 | 0 | 614 | 0
  mov r10, 819
  movq xmm7, r10 ; Copio el 819
  shufps xmm7, xmm7, 0 ; Lo extiendo
  ; xmm7(W) = 819 | 0 | 819 | 0 | 819 | 0 | 819 | 0
  pcmpeqd xmm12, xmm12 ; Todo en 1
  ; xmm12(W) = 0xFFFF | 0xFFFF | 0xFFFF | 0xFFFF | 0xFFFF | 0xFFFF | 0xFFFF | 0xFFFF
  mov r10, 0xFFFF
  movq xmm13, r10 ; Copio el 0xFFFF
  shufps xmm13, xmm13, 0 ; Lo extiendo
  ; xmm13(W) = 0xFFFF | 0 | 0xFFFF | 0 | 0xFFFF | 0 | 0xFFFF | 0
  movups xmm15, [MASK_PSHUFB]
  ; xmm13(B) = MASK_PSHUFB
  ; Usandolo con PSHUFB pone todos los bytes pares en la mitad menos significativa y 0 en el resto
  
  
  ; HACK FIXME
  mov rax, r14
  shr rax, 1
  jnc .vars
  dec r14
  
  .vars:
  ; Obtengo el resto de w / 8 (la cantidad de pixels que proceso por vez)
  mov eax, r15d
  mov edx, eax
  shr edx, 16
  mov cx, 8
  idiv cx
  ; El resto queda en DX
  movsx edx, dx
  movsx rdx, edx
  ; RDX = (int) alineacion
  mov rcx, rdx
  ; RCX = (bool) checkearAlineamiento
  mov rax, 0
  ; RAX = (int) filaActual
  mov r13, 0
  
  .loop:
  mov r10, rdi
  add r10, r8 ; rdi + 1 linea
  
  mov r11, rsi
  add r11, r9 ; rsi + 1 linea
  

  ; Traigo los pixels
  movq xmm0, [rdi] ; 8 pixels línea 0
  ; xmm0(B) = l0p0 | l0p1 | l0p2 | l0p3 | l0p4 | l0p5 | l0p6 | l0p7 | b | b | b | b | b | b | b | b |
  movq xmm1, [r10] ; 8 pixels línea 1
  ; xmm1(B) = l1p0 | l1p1 | l1p2 | l1p3 | l1p4 | l1p5 | l1p6 | l1p7 | b | b | b | b | b | b | b | b |
  
  ; Unpack
  punpcklbw xmm0, xmm3
  ; xmm0(W) = l0p0 | l0p1 | l0p2 | l0p3 | l0p4 | l0p5 | l0p6 | l0p7
  punpcklbw xmm1, xmm3
  ; xmm1(W) = l1p0 | l1p1 | l1p2 | l1p3 | l1p4 | l1p5 | l1p6 | l1p7
  
  ; Sumo
  paddw xmm0, xmm1 ; Sumo cada pixel verticalmente
  ; xmm0(W) = l0p0+l1p0 | l0p1+l1p1 | l0p2+l1p2 | l0p3+l1p3 | l0p4+l1p4 | l0p5+l1p5 | l0p6+l1p6 | l0p7+l1p7
  movaps xmm1, xmm0 ; xmm1 = xmm0
  ; xmm1(W) = l0p0+l1p0 | l0p1+l1p1 | l0p2+l1p2 | l0p3+l1p3 | l0p4+l1p4 | l0p5+l1p5 | l0p6+l1p6 | l0p7+l1p7
  psrldq xmm1, 2  ; Shifteo una word a la derecha para alinear
  ; xmm1(W) = l0p1+l1p1 | l0p2+l1p2 | l0p3+l1p3 | l0p4+l1p4 | l0p5+l1p5 | l0p6+l1p6 | l0p7+l1p7 | 0
  paddw xmm0, xmm1 ; Sumo las sumas de pixeles horizontalmente
  ; xmm0(W) = l0p0+l1p0+l0p1+l1p1 | l0p1+l1p1+l0p2+l1p2 | l0p2+l1p2+l0p3+l1p3 | l0p3+l1p3+l0p4+l1p4 | l0p4+l1p4+l0p5+l1p5 | l0p5+l1p5+l0p6+l1p6 | l0p6+l1p6+l0p7+l1p7 | l0p7+l1p7+0
  
  ; Comparo
  movaps xmm8, xmm0 ; xmm8 = xmm0
  pcmpgtw xmm8, xmm4 ; Máscara si es >= 205
  
  movaps xmm9, xmm0 ; xmm9 = xmm0
  pcmpgtw xmm9, xmm5 ; Máscara si es >= 410
  
  movaps xmm10, xmm0 ; xmm10 = xmm0
  pcmpgtw xmm10, xmm6 ; Máscara si es >= 615
  
  movaps xmm11, xmm0 ; xmm11 = xmm0
  pcmpgtw xmm11, xmm7 ; Máscara si es >= 820
  
  ; Unir y Pack
  ; Línea 0
  movaps xmm0, xmm8 ; xmm0 = xmm8
  pand xmm0, xmm13 ; Pongo ceros en dónde hay resultados basura
  
  movaps xmm1, xmm11 ; xmm1 = xmm11
  pand xmm1, xmm13 ; Pongo ceros en dónde hay resultados basura
  pslldq xmm1, 2 ; Shifteo una word a la izquierda para alinear
  
  por xmm0, xmm1 ; Reemplazo los ceros de xmm0 por los valores de xmm1  
  pshufb xmm0, xmm15; Ordeno
  
  ; Línea 1
  movaps xmm1, xmm10 ; xmm1 = xmm10
  pand xmm1, xmm13 ; Pongo ceros en dónde hay resultados basura
  
  movaps xmm14, xmm9 ; xmm14 = xmm9
  pand xmm14, xmm13 ; Pongo ceros en dónde hay resultados basura
  pslldq xmm14, 2 ; Shifteo una word a la izquierda para alinear
  
  por xmm1, xmm14 ; Reemplazo los ceros de xmm1 por los valores de xmm14  
  pshufb xmm1, xmm15; Ordeno
  
  ; Pongo los pixeles
  movq [rsi], xmm0 ; 8 pixels línea 0
  movq [r11], xmm1 ; 8 pixels línea 1
  
  ; Checkeo si hay que alinear
  cmp rcx, 0
  je .noAlinear
  add rdi, rdx
  add rsi, rdx
  add r13, rdx
  mov rcx, 0 ; Marco para no alinear de nuevo
  jmp .loop
  
  .noAlinear:
  add rdi, 8 ; Avanzo 8 pixels
  add rsi, 8
  add r13, 8
  cmp r13, r15
  jl .loop ; Si no terminé la fila sigo
  
  mov rcx, rdx ; Marco para ver si hay que alinear la próxima fila
  add rax, 2 ; Avanzo 2 filas
  ; Resto la w y sumo 2 veces [src|dst]_row_size
  mov r13, 0
  sub rdi, r15
  add rdi, r8
  add rdi, r8
  sub rsi, r15
  add rsi, r9
  add rsi, r9
  cmp rax, r14
  jl .loop ; Si quedan filas sigo
  
  pop r15
  pop r14
  pop r13
  pop r12
  pop rbp
	ret
