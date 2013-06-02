; void colorizar_asm (
; 	unsigned char *src,
; 	unsigned char *dst,
; 	int h,
; 	int w,
; 	int src_row_size,
; 	int dst_row_size,
;   float alpha
; );

; Parámetros:
; 	rdi = src
; 	rsi = dst
; 	edx = h
; 	ecx = w
; 	r8d = src_row_size
; 	r9d = dst_row_size
;   xmm0 = alpha


global colorizar_asm

; Calcula el máximo de a 3 dado un registro con 3 pixels a partir del 2° byte
; Toma el regsitro en %1 y lo devuelve en %2, con la primer dword con basura
; Utiliza como tmp a xmm3, xmm4 y xmm5
%macro maximos 2
  pxor %2, %2
  movaps xmm3, %1
  punpcklbw xmm3, %2
  ; xmm3(W) = B | p1b | p1g | p1r | p2b | p2g | p2r | p3b
  movaps xmm4, xmm3
  punpcklwd xmm4, %2
  ; xmm4(DW) = B | p1b | p1g | p1r
  psrldq xmm3, 6
  ; xmm3(W) = B | p2b | p2g | p2r | p3b | 0 | 0 | 0
  movaps xmm5, xmm3
  punpcklwd xmm5, %2
  ; xmm5(DW) = B | p2b | p2g | p2r
  pmaxud xmm4, xmm5
  ; xmm4(DW) = B | maxB(1,2) | maxG(1,2) | maxR(1,2)
  movaps xmm3, %1
  ; xmm3(B) = B | p1b | p1g | p1r | p2b | p2g | p2r | p3b | p3g | p3r | p4b | p4g | p4r | p5b | p5g | p5r
  psrldq xmm3, 6
  ; xmm3(B) = B | p3b | p3g | p3r | p4b | p4g | p4r | p5b | p5g | p5r | 0 | 0 | 0 | 0 | 0 | 0
  movaps xmm5, xmm3
  punpcklbw xmm5, %2
  ; xmm5(W) = B | p3b | p3g | p3r | p4b | p4g | p4r | p5b
  punpcklwd xmm5, %2
  ; xmm5(DW) = B | p3b | p3g | p3r
  pmaxud xmm4, xmm5
  ; xmm4(DW) = B | maxB(1,2,3) | maxG(1,2,3) | maxR(1,2,3)
  movaps %2, xmm4
%endmacro

; Dados 3 registros con 3 pixels a partir del 2° byte, interpretados como 3 líneas
; Aplica la función colorizar para el pixel del centro
; Y devuelve sus valores en %1
; Utiliza como tmp a xmm3, xmm4, xmm5, xmm6, xmm8 y xmm9
%macro colorizar_pixel 1
  ; Máximos
  maximos xmm0, xmm6 ; Fila anterior
  ; xmm6(DW) = B | maxB(1,2,3) | maxG(1,2,3) | maxR(1,2,3)
  maximos xmm1, %1 ; Fila actual
  pmaxud xmm6, %1
  ; xmm6(DW) = B | maxB(1,2,3,4,5,6) | maxG(1,2,3,4,5,6) | maxR(1,2,3,4,5,6)
  maximos xmm2, %1 ; Fila siguiente
  pmaxud xmm6, %1
  ; xmm6(DW) = B | maxB(1,2,3,4,5,6,7,8,9) | maxG(1,2,3,4,5,6,7,8,9) | maxR(1,2,3,4,5,6,7,8,9)
  
  ; Traigo los Isrc
  pxor %1, %1
  movaps xmm3, xmm1
  punpcklbw xmm3, %1
  ; xmm3(W) = B | p1b | p1g | p1r | p2b | p2g | p2r | p3b
  punpckhwd xmm3, %1
  ; xmm3(DW) = p2b | p2g | p2r | B ; Este es el pixel que estoy procesando ahora
  
  ; Convierto a float
  cvtdq2ps xmm6, xmm6
  cvtdq2ps xmm3, xmm3
  
  ; Acomodo para comparar
  movaps %1, xmm6
  shufps xmm6, xmm6, 0b00111011
  ; xmm6(DW) = maxR | maxG | maxR | B
  shufps %1, %1, 0b00100101
  ; %1(DW) = maxB | maxB | maxG | B
  
  ; Comparo
  cmpleps %1, xmm6 ; Comparo por menor o igual
  ; %1(DW) = maxB <= maxR | maxB <= maxG | maxG <= maxR | B
  
  ; Acomodo y obtengo los phi
  movaps xmm6, %1
  shufps %1, %1, 0b11111110
  ; %1(DW) = maxG <= maxR | B | B | B
  pand %1, xmm6
  ; %1(DW) = maxG <= maxR && maxb <= maxR | B | B | B ; phiR
  pslldq %1, 8
  ; %1(DW) = 0 | 0 | phiR | B
  movaps xmm8, xmm6
  shufps xmm8, xmm8, 0b11111011
  ; xmm8(DW) = B | maxG <= maxR | B | B
  pandn xmm8, xmm6
  ; xmm8(DW) = B | !(maxG <= maxR) && maxB <= maxG  | B | B ; phiG
  pslldq xmm8, 8
  psrldq xmm8, 8
  ; xmm8(DW) = B | phiG  | 0 | 0
  por %1, xmm8
  ; %1(DW) = B | phiG | phiR | B
  movaps xmm8, xmm6
  movaps xmm9, xmm6
  pandn xmm9, xmm13 ; AND NOT con una máscara de 1s es un NOT
  ; xmm9(DW) = !(maxB <= maxR) | !(maxB <= maxG) | !(maxG <= maxR) | B
  shufps xmm8, xmm8, 0b11111101
  ; xmm8(DW) = maxB <= maxG | B | B | B
  pandn xmm8, xmm9
  ; xmm8(DW) = !(maxB <= maxG) && !(maxB <= maxR) | B | B | B ; phiB
  pslldq xmm8, 12
  psrldq xmm8, 12
  ; xmm8(DW) = phiB | 0 | 0 | 0
  psrldq %1, 4
  pslldq %1, 4
  ; %1(DW) = 0 | phiG | phiR | B
  por %1, xmm8
  ; %1(DW) = phiB | phiG | phiR | B
  ; Ahora tengo las condiciones de los phi, si son verdaderas hay que sumar alpha, sino restar
  ; Sé que alpha está entre 0 y 1, por lo tanto su signo es positivo
  ; Para simplificar cambio src * (1 +/- alpha) por src + src * (+/-) alpha
  ; Entonces solo tengo que ponerle un 1 al signo de la máscara de alphas antes de multiplicar
  pandn %1, xmm14
  ; %1(DW) = !phiB (solo el bit del signo) | !phiG (solo el bit del signo) | !phiR (solo el bit del signo) | B
  ; Al negar la condición y hacer un AND con la máscara del bit de signo, me queda
  ; un 1 en el bit del signo si la condición era falsa (es decir, si tenía que restar alpha)
  por %1, xmm15
  ; %1(DW) = (+/-)alphaB | (+/-)alphaG | (+/-)alphaR | B
  mulps %1, xmm3
  ; %1(DW) = srcB * (+/-)alphaB | srcG * (+/-)alphaG | srcR * (+/-)alphaR | B
  addps %1, xmm3
  ; %1(DW) = srcB * (1 +/- alphaB) | srcG * (1 +/- alphaG) | srcR * (1 +/- alphaR) | B
  
  ; Pongo en un destino tmp
  cvtps2dq %1, %1 ; Convierto a int
  packusdw %1, %1 ; Pack con saturación a word
  packuswb %1, %1 ; Pack con saturación a byte
  ; %1(B) = dstB | dstG | dstR | B | B | B | B | B | B | B | B | B | B | B | B | B
  pslldq %1, 13
  psrldq %1, 13
  ; %1(B) = dstB | dstG | dstR | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
%endmacro

section .rodata
  MASK_SIGN_FLOAT: DB 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80


section .text

colorizar_asm:
	push rbp
	mov rbp, rsp
	push r12
	push r13
	push r14
	push r15
	
	; Extiendo para después hacer operaciones más fácil
  movsx r10, ecx; R10 = w
  movsx r11, edx ; R11 = h
  movsx r8, r8d ; R8 = src_row_size
	movsx r9, r9d ; R9 = dst_row_size
	
	; Obtengo el resto de R10 / 3 (la cantidad de pixels que proceso por vez)
	mov rax, r10
	sub rax, 2 ; Porque no proceso las puntas
	xor rdx, rdx ; Funciona porque R10 siempre es positivo
	mov rcx, 3
	idiv rcx ; rdx:rax / rcx; Cociente en rax, resto en rdx
	mov rax, 3 ; Cantidad de bytes por pixel
	mul rdx ; rax = rdx*3
	mov r12, rax ; R12 = Cantidad de bytes a alinear
	mov r13, r12 ; R13 = (bool) Si hay que alinear
	
	; Acomodo registros para tener en cuenta que no se procesa ni la primera
	; ni la última fila/columna
	mov rax, 3 ; Cantidad de bytes por pixel
	mul r10
	mov r10, rax ; R10 = w * 3; Cantidad de bytes totales por fila
	sub r10, 16 ; R10 = w * 3 - 16; Le resto 16 bytes para tener el último dqword que tengo que traer
	
	sub r11, 2 ; R11 = h - 2
	
	; Offsets	
	xor r14, r14 ; Fila
	xor r15, r15 ; Columna para src
	xor rcx, rcx ; Clomuna para dst
	
	
	; Máscaras
	movaps xmm15, xmm0 ; Copio el valor de alpha
	shufps xmm15, xmm15, 0
	; xmm15(DW) = alpha | alpha | alpha | alpha
	movups xmm14, [MASK_SIGN_FLOAT]
	; xmm14(DW) = 0x80000000 | 0x80000000 | 0x80000000 | 0x80000000
	mov rax, 0xFFFFFFFF
	movq xmm13, rax
	shufps xmm13, xmm13, 0
	; xmm13 = todos 1s

  .loopFilas:
    ; Traigo los primeros 16 bytes en 3 registros xmm (fila anterior, a procesar y siguiente)
    mov rax, r14 ; Pongo la fila
    mul r8 ; Multiplico por src_row_size para obtener => Offset: r14 filas
    movups xmm0, [rdi+rax]
    add rax, r8 ; Offset: r14+1 filas
    movups xmm1, [rdi+rax]
    add rax, r8 ; Offset: r14+2 filas
    movups xmm2, [rdi+rax]
    ; Shifteo un byte para que en la posición menos significativa quede basura
    ; y el resto sean 5 pixels
    pslldq xmm0, 1
    pslldq xmm1, 1
    pslldq xmm2, 1
    jmp .loopColumnasAfterGetting
  .loopColumnas:
    ; Traigo los primeros 16 bytes en 3 registros xmm (fila anterior, a procesar y siguiente)
    mov rax, r14 ; Pongo la fila
    mul r8 ; Multiplico por src_row_size para obtener => Offset: r14 filas
    add rax, r15 ; Sumo las columnas para obtener => Offset: r14 filas + r15 columnas
    movups xmm0, [rdi+rax]
    add rax, r8 ; Offset: r14+1 filas + r15 columnas
    movups xmm1, [rdi+rax]
    add rax, r8 ; Offset: r14+2 filas + r15 columnas
    movups xmm2, [rdi+rax]
    
    .loopColumnasAfterGetting:
    ; Los registros xmm0, xmm1 y xmm2 quedan así (para las filas 1, 2 y 3):
    ; B | p1b | p1g | p1r | p2b | p2g | p2r | p3b | p3g | p3r | p4b | p4g | p4r | p5b | p5g | p5r
  
    ; Proceso
    ; Primer pixel
    colorizar_pixel xmm10
    ; xmm10(B) = dst1B | dst1G | dst1R | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
    ; Segundo pixel
    psrldq xmm0, 3
    psrldq xmm1, 3
    psrldq xmm2, 3
    colorizar_pixel xmm7
    ; xmm7(B) = dst2B | dst2G | dst2R | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
    pslldq xmm7, 3
    por xmm10, xmm7
    ; xmm10(B) = dst1B | dst1G | dst1R | dst2B | dst2G | dst2R | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
    ; Tercer pixel
    psrldq xmm0, 3
    psrldq xmm1, 3
    psrldq xmm2, 3
    colorizar_pixel xmm7
    ; xmm7(B) = dst3B | dst3G | dst3R | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
    pslldq xmm7, 6
    por xmm10, xmm7
    ; xmm10(B) = dst1B | dst1G | dst1R | dst2B | dst2G | dst2R | dst3B | dst3G | dst3R  | 0 | 0 | 0 | 0 | 0 | 0 | 0
    
    ; Muevo a memoria
    
    ; Checkeo si son los primeros 3 pixels que son un caso especial
    cmp rcx, 0
    je .primerosPixels
    ; Si no
    pslldq xmm10, 7
    ; xmm10(B) = 0 | 0 | 0 | 0 | 0 | 0 | 0 | dst1B | dst1G | dst1R | dst2B | dst2G | dst2R | dst3B | dst3G | dst3R
    ; En xmm11 tengo los últimos 7 de la pasada anterior
    por xmm11, xmm10
    ; xmm11(B) = old | old | old | old | old | old | old | dst1B | dst1G | dst1R | dst2B | dst2G | dst2R | dst3B | dst3G | dst3R
    mov rax, r14 ; Pongo la fila
    inc rax ; Sumo 1 porque r14 apunta a la fila anterior
    mul r9 ; Multiplico por dst_row_size para obtener => Offset: r14+1 filas
    add rax, rcx ; Sumo las columnas => Offset: r14+1 filas + rcx columnas
    movups [rsi+rax], xmm11
    jmp .checkearAlineamiento
    
    .primerosPixels:    
    mov rax, r14 ; Pongo la fila
    inc rax ; Sumo 1 porque r14 apunta a la fila anterior
    mul r9 ; Multiplico por dst_row_size para obtener => Offset: r14+1 filas
    add rax, 3 ; Sumo las columnas => Offset: r14+1 filas + 3 columnas para dejar 1 pixel libre
    movups [rsi+rax], xmm10
    ; Lo copio y acomodo en xmm11 para usarlo en el próximo paso
    movaps xmm11, xmm10
    psrldq xmm11, 2
    ; xmm11(B) = dst1R | dst2B | dst2G | dst2R | dst3B | dst3G | dst3R | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
    mov r15, 8 ; El segundo empieza en 8 para traer 1 byte basura y 5 pixels
    mov rcx, 5 ; El segundo pone desde 5 para poner 7 bytes repetidos y 3 pixels nuevos
    jmp .loopColumnas
    ; No alineo despues de los primeros 3 pixels porque el resto podría no entrar en 16 bytes
    
    
    .checkearAlineamiento:
    ; Checkeo si hay que alinear
    cmp r13, 0
    je .noAlinear
    ;jmp .fin ; AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    mov r13, 0 ; Marco para no alinear de nuevo
    cmp r12, 3 ; Si el resto == 3 bytes (1 pixel)
    jne .restoIgual2
    mov rcx, 8;add rcx, 3
    mov r15, 11;sub r15, 6
    ; Acomodo xmm11 para usarlo en el próximo paso
    pslldq xmm11, 6
    psrldq xmm11, 9
    ; xmm11(B) = old | old | old | old | dst1B | dst1G | dst1R | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
    jmp .loopColumnas
    .restoIgual2: ; Si el resto == 6 bytes (2 pixels)
    mov rcx, 11;add rcx, 6
    mov r15, 14;sub r15, 3
    ; Acomodo xmm11 para usarlo en el próximo paso
    pslldq xmm11, 3
    psrldq xmm11, 9
    ; xmm11(B) = old | dst1B | dst1G | dst1R | dst2B | dst2G | dst2R | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
    jmp .loopColumnas
    
    .noAlinear:
    add r15, 9 ; Avanzo de a 9 bytes = 3 bytes
    add rcx, 9 ; Avanzo de a 9 bytes = 3 bytes
    ; Acomodo xmm11 para usarlo en el próximo paso
    psrldq xmm11, 9
    ; xmm11(B) = dst1R | dst2B | dst2G | dst2R | dst3B | dst3G | dst3R | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
    cmp r15, r10
    jle .loopColumnas ; Si no terminé la fila sigo
    
    mov r13, r12 ; Marco para ver si hay que alinear la próxima fila
    xor r15, r15 ; Reseteo los offsets a columnas
    xor rcx, rcx
    inc r14 ; Avanzo 1 fila
    cmp r14, r11
    jl .loopFilas ; Si quedan filas sigo
    
  .fin:
  pop r15
  pop r14
  pop r13
  pop r12
  pop rbp
	ret

