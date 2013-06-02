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

; Aplica el proceso de halftone de a 2 líneas de 8 pixels
; En los comentarios se muestra %1 como xmm0 y %2 como xmm1 para simular la primera llamada
; Deja el resultado en la parte menos significativa de cada parámetro y el resto en 0 para unir con POR
%macro halftone_pixels 2
  ; Sumo
  paddw %1, %2 ; Sumo cada pixel verticalmente
  ; xmm0(W) = l0p0+l1p0 | l0p1+l1p1 | l0p2+l1p2 | l0p3+l1p3 | l0p4+l1p4 | l0p5+l1p5 | l0p6+l1p6 | l0p7+l1p7
  movaps %2, %1 ; xmm1 = xmm0
  ; xmm1(W) = l0p0+l1p0 | l0p1+l1p1 | l0p2+l1p2 | l0p3+l1p3 | l0p4+l1p4 | l0p5+l1p5 | l0p6+l1p6 | l0p7+l1p7
  psrldq %2, 2  ; Shifteo una word a la derecha para alinear
  ; xmm1(W) = l0p1+l1p1 | l0p2+l1p2 | l0p3+l1p3 | l0p4+l1p4 | l0p5+l1p5 | l0p6+l1p6 | l0p7+l1p7 | 0
  paddw %1, %2 ; Sumo las sumas de pixeles horizontalmente
  ; xmm0(W) = l0p0+l1p0+l0p1+l1p1 | l0p1+l1p1+l0p2+l1p2 | l0p2+l1p2+l0p3+l1p3 | l0p3+l1p3+l0p4+l1p4 | l0p4+l1p4+l0p5+l1p5 | l0p5+l1p5+l0p6+l1p6 | l0p6+l1p6+l0p7+l1p7 | l0p7+l1p7+0
  
  ; Comparo
  movaps xmm8, %1 ; xmm8 = xmm0
  pcmpgtw xmm8, xmm4 ; Máscara si es >= 205
  
  movaps xmm9, %1 ; xmm9 = xmm0
  pcmpgtw xmm9, xmm5 ; Máscara si es >= 410
  
  movaps xmm10, %1 ; xmm10 = xmm0
  pcmpgtw xmm10, xmm6 ; Máscara si es >= 615
  
  movaps xmm11, %1 ; xmm11 = xmm0
  pcmpgtw xmm11, xmm7 ; Máscara si es >= 820
  
  ; Unir y Pack
  ; Línea 0
  movaps %1, xmm8 ; xmm0 = xmm8
  pand %1, xmm13 ; Pongo ceros en dónde hay resultados basura
  
  movaps %2, xmm11 ; xmm1 = xmm11
  pand %2, xmm13 ; Pongo ceros en dónde hay resultados basura
  pslldq %2, 2 ; Shifteo una word a la izquierda para alinear
  
  por %1, %2 ; Reemplazo los ceros de xmm0 por los valores de xmm1  
  pshufb %1, xmm15; Ordeno
  
  ; Línea 1
  movaps %2, xmm10 ; xmm1 = xmm10
  pand %2, xmm13 ; Pongo ceros en dónde hay resultados basura
  
  movaps xmm14, xmm9 ; xmm14 = xmm9
  pand xmm14, xmm13 ; Pongo ceros en dónde hay resultados basura
  pslldq xmm14, 2 ; Shifteo una word a la izquierda para alinear
  
  por %2, xmm14 ; Reemplazo los ceros de xmm1 por los valores de xmm14  
  pshufb %2, xmm15; Ordeno
%endmacro

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
  mov r10, 0xFFFF
  movq xmm13, r10 ; Copio el 0xFFFF
  shufps xmm13, xmm13, 0 ; Lo extiendo
  ; xmm13(W) = 0xFFFF | 0 | 0xFFFF | 0 | 0xFFFF | 0 | 0xFFFF | 0
  movups xmm15, [MASK_PSHUFB]
  ; xmm13(B) = MASK_PSHUFB
  ; Usandolo con PSHUFB pone todos los bytes pares en la mitad menos significativa y 0 en el resto
  
  
  ; Tomo solo la altura par
  mov rax, r14
  shr rax, 1
  jnc .anchoPar
  dec r14
  
  ; Tomo solo el ancho par
  .anchoPar:
  mov rax, r15
  shr rax, 1
  jnc .vars
  dec r15
  
  .vars:
  ; Obtengo el resto de w / 16 (la cantidad de pixels que proceso por vez)
  mov eax, r15d
  mov edx, eax
  shr edx, 16
  mov cx, 16
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
  movups xmm0, [rdi] ; 16 pixels línea 0
  ; xmm0(B) = l0p0 | l0p1 | l0p2 | l0p3 | l0p4 | l0p5 | l0p6 | l0p7 | l0p8 | l0p9 | l0p10 | l0p11 | l0p12 | l0p13 | l0p14 | l0p15
  movups xmm1, [r10] ; 16 pixels línea 1
  ; xmm1(B) = l1p0 | l1p1 | l1p2 | l1p3 | l1p4 | l1p5 | l1p6 | l1p7 | l1p8 | l1p9 | l1p10 | l1p11 | l1p12 | l1p13 | l1p14 | l1p15
  movaps xmm2, xmm0 ; 16 pixels línea 0
  ; xmm2(B) = l0p0 | l0p1 | l0p2 | l0p3 | l0p4 | l0p5 | l0p6 | l0p7 | l0p8 | l0p9 | l0p10 | l0p11 | l0p12 | l0p13 | l0p14 | l0p15
  movaps xmm12, xmm1 ; 16 pixels línea 1
  ; xmm12(B) = l1p0 | l1p1 | l1p2 | l1p3 | l1p4 | l1p5 | l1p6 | l1p7 | l1p8 | l1p9 | l1p10 | l1p11 | l1p12 | l1p13 | l1p14 | l1p15
  
  ; Unpack
  punpcklbw xmm0, xmm3
  ; xmm0(W) = l0p0 | l0p1 | l0p2 | l0p3 | l0p4 | l0p5 | l0p6 | l0p7
  punpcklbw xmm1, xmm3
  ; xmm1(W) = l1p0 | l1p1 | l1p2 | l1p3 | l1p4 | l1p5 | l1p6 | l1p7
  punpckhbw xmm2, xmm3
  ; xmm2(W) = l0p8 | l0p9 | l0p10 | l0p11 | l0p12 | l0p13 | l0p14 | l0p15
  punpckhbw xmm12, xmm3
  ; xmm12(W) = l1p8 | l1p9 | l1p10 | l1p11 | l1p12 | l1p13 | l1p14 | l1p15
  
  halftone_pixels xmm0, xmm1
  halftone_pixels xmm2, xmm12
  pslldq xmm2, 8 ; Shifteo 4 words a la izquierda para acomodar
  pslldq xmm12, 8 ; Shifteo 4 words a la izquierda para acomodar
  por xmm0, xmm2
  por xmm1, xmm12
  
  
  ; Pongo los pixeles
  movups [rsi], xmm0 ; 16 pixels línea 0
  movups [r11], xmm1 ; 16 pixels línea 1
  
  ; Checkeo si hay que alinear
  cmp rcx, 0
  je .noAlinear
  add rdi, rdx
  add rsi, rdx
  add r13, rdx
  mov rcx, 0 ; Marco para no alinear de nuevo
  jmp .loop
  
  .noAlinear:
  add rdi, 16 ; Avanzo 16 pixels
  add rsi, 16
  add r13, 16
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
