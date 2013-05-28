; void recortar_asm (
; 	unsigned char *src,
; 	unsigned char *dst,
; 	int h,
; 	int w,
; 	int src_row_size,
; 	int dst_row_size,
; 	int tam
; );

; Parámetros:
; 	rdi = src
; 	rsi = dst
; 	edx = h
; 	ecx = w
; 	r8d = src_row_size
; 	r9d = dst_row_size
; 	rbp + 16 = tam
%define VAR_TAM rbp + 16

global recortar_asm

section .text

recortar_asm:
	push rbp
	mov rbp, rsp
	push r12
	push r13
	push r14
	push r15
	
  ; Extiendo para después hacer operaciones más fácil
  movsx rcx, ecx
  movsx rdx, edx
	movsx r8, r8d
	movsx r9, r9d
	movsx r10, dword[VAR_TAM] ; Tamaño
	
	
	mov r11, 0 ; Fila
	mov r12, 0 ; Columna
	mov r13, rcx
	sub r13, r10 ; w - tam
	mov r14, rdx
	sub r14, r10 ; h - tam
	
	; Obtengo el resto de tam / 16 (la cantidad de pixels que proceso por vez)
	mov rax, r10
  mov edx, eax
  shr edx, 16
  mov cx, 16
  idiv cx
  ; El resto queda en DX
  movsx edx, dx
  movsx r15, edx
  ; R15 = (int) alineacion
  mov rcx, r15
  ; RCX = (bool) checkearAlineamiento
	
	.loop:
	
	; Traigo 16 pixels parte A
	mov rax, r11 ; Pongo la fila
	mul r8; Multiplico por src_row_size para apuntar al principio de esa fila
	add rax, r12; Sumo para llegar a la columna correcta
	movups xmm0, [rdi+rax]
	; Los pongo en la parte D
	mov rax, r11 ; Pongo la fila
	add rax, r10 ; Sumo tam para apuntar a la 2° mitad verticalmente
	mul r9 ; Multiplico por dst_row_size para apuntar al principio de esa fila
	add rax, r10 ; Sumo tam para apuntar a la 2° mitad horizontalmente
	add rax, r12 ; Sumo para llegar a la columna correcta
	movups [rsi+rax], xmm0
	
	; Traigo 16 pixels parte B
	mov rax, r11 ; Pongo la fila
	mul r8; Multiplico por src_row_size para apuntar al principio de esa fila
	add rax, r13 ; Sumo w - tam para llegar al comienzo de la parte B
	add rax, r12 ; Sumo para llegar a la columna correcta
	movups xmm0, [rdi+rax]
	; Los pongo en la parte C
	mov rax, r11 ; Pongo la fila
	add rax, r10 ; Sumo tam para apuntar a la 2° mitad verticalmente
	mul r9; Multiplico por dst_row_size para apuntar al principio de esa fila
	add rax, r12 ; Sumo para llegar a la columna correcta
	movups [rsi+rax], xmm0
	
	; Traigo 16 pixels parte C
	mov rax, r11 ; Pongo la fila
	add rax, r14 ; Sumo h - tam para llegar al comienzo de la parte C
	mul r8; Multiplico por src_row_size para apuntar al principio de esa fila
	add rax, r12 ; Sumo para llegar a la columna correcta
	movups xmm0, [rdi+rax]
	; Los pongo en la parte B
	mov rax, r11 ; Pongo la fila
	mul r9; Multiplico por dst_row_size para apuntar al principio de esa fila
	add rax, r10 ; Sumo tam para apuntar a la 2° mitad horizontalmente
	add rax, r12 ; Sumo para llegar a la columna correcta
	movups [rsi+rax], xmm0
	
	; Traigo 16 pixels parte D
	mov rax, r11 ; Pongo la fila
	add rax, r14 ; Sumo h - tam para llegar al comienzo de la parte D
	mul r8; Multiplico por src_row_size para apuntar al principio de esa fila
	add rax, r13 ; Sumo w - tam para llegar al comienzo de la parte D
	add rax, r12 ; Sumo para llegar a la columna correcta
	movups xmm0, [rdi+rax]
	; Los pongo en la parte A
	mov rax, r11 ; Pongo la fila
	mul r9; Multiplico por dst_row_size para apuntar al principio de esa fila
	add rax, r12 ; Sumo para llegar a la columna correcta
	movups [rsi+rax], xmm0
	
	
	
	; Checkeo si hay que alinear
  cmp rcx, 0
  je .noAlinear
  add r12, r15
  mov rcx, 0 ; Marco para no alinear de nuevo
  jmp .loop
  
  .noAlinear:
  add r12, 16 ; Avanzo 16 pixels
  cmp r12, r10
  jl .loop ; Si no terminé la fila sigo
  
  mov rcx, r15 ; Marco para ver si hay que alinear la próxima fila
  inc r11 ; Avanzo 1 fila
  mov r12, 0 ; Reseteo la columna
  cmp r11, r10
  jl .loop ; Si quedan filas sigo
	
	pop r15
	pop r14
	pop r13
  pop r12
  pop rbp
	ret
