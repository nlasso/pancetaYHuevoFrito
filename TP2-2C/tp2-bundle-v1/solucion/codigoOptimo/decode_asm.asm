global decode_asm

section .data


section .text
;void decode_asm(unsigned char *src, (rdi)
;              unsigned char *code, (rsi)
;              int size, (rdx)
;              int width, (rcx)
;              int height); (r8)

decode_asm:
	push rbp ; Pila Allineada
	mov rbp, rsp
	push rbx ; Pila Desalineada
	push r12
	push r13
	push r14
	push r15

	mov r12, rdx
	dec r12
	mov rax, 3
	mul rcx
	mul r8 ; cantDe Bytes en eax
	xor r8, r8
	mov r8d, eax
	sub r8, 16
	mov r11, 0 ; Bytes leidos

.ciclo:

	PXOR xmm0, xmm0 ; xmm0 = 0..0
	MOVDQU xmm0, [rdi] ; Obtengo 16 bytes
	MOVDQA xmm1, xmm0

	mov ebx, 0c0c0c0ch ; Para quedarme con los bits 2 y 3
	movd xmm1, ebx
	shufps xmm1, xmm1, 0h
	pand xmm1, xmm0
	PSRLW xmm1, 2;SHIFTEAR


	
	MOVDQA xmm2, xmm1 ; Guardo los bits op en xmm2
	MOVDQA xmm3, xmm1 ; Guardo los bits op en xmm3
	MOVDQA xmm4, xmm1 ; Guardo los bits op en xmm4

	MOV EBX,01010101h
	movd xmm5, ebx
	shufps xmm5, xmm5, 0h

	pand xmm2, xmm5
	PSRLW xmm1, 1
	PAND xmm1, xmm2
	PXOR xmm1, xmm2 ; En xmm1 quedó la máscara para op = 1

	PSRLW xmm4, 1
	PAND xmm4, xmm5 ; Me quedo con el bit más significativo del op
	PANDN xmm2, xmm4 ; Niego los bits op y luego hago and con xmm4. Me quedan en 1 los bytes cuyo op original era 2
					 ; En xmm2 quedó la máscara para op = 2



	MOVDQA xmm13, xmm0
	MOVDQA xmm14, xmm0
	MOV r13d, 0b0b0b0bh
	movd xmm6, r13d
	shufps xmm6, xmm6, 0h
	MOV r14d, 03030303h
	movd xmm7, r14d
	shufps xmm7, xmm7, 0h
	MOV r15d, 0xFFFFFFFF
	movd xmm8, r15d
	shufps xmm8, xmm8, 0h

	MOV r10d, 0f0f0f0fh
	movd xmm10, r10d
	shufps xmm10, xmm10, 0h
	pand xmm0, xmm10
	movdqa xmm13, xmm0
	movdqa xmm14, xmm0

	pcmpgtb xmm0, xmm6
	pand xmm13, xmm0
	pand xmm7, xmm0
	pxor xmm0, xmm8
	pand xmm14, xmm0
	pxor xmm13, xmm7
	paddb xmm13, xmm14
	MOVDQA xmm3, xmm13 ; En xmm3 queda aplicada la negacion para los op 3

	paddb xmm3, xmm1 ; Sumo 1 a los correspondientes
	psubb xmm3, xmm2 ; Resto 1 a los correspondientes

	MOV ebx, 03030303h
	movd xmm5, ebx
	shufps xmm5, xmm5, 0h
	pand xmm3, xmm5 ; Borro todos los bits que no me interesan


	MOVDQA xmm0, xmm3
	MOVDQA xmm1, xmm3
	MOVDQA xmm2, xmm3

;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	
	MOV ebx, 00000003h ; Dejo en xmm0 los bits que seran 0 y 1 de cada byte
	MOV r10d, 00000300h ; Dejo en xmm1 los bits que seran 2 y 3 de cada byte
	MOV r13d, 00030000h ; Dejo en xmm2 los bits que seran 4 y 5 de cada byte
	MOV r14d, 03000000h ; Dejo en xmm3 los bits que seran 6 y 7 de cada byte

	movd xmm5, ebx
	movd xmm6, r10d
	movd xmm7, r13d
	movd xmm8, r14d

	shufps xmm5, xmm5, 0h
	shufps xmm6, xmm6, 0h
	shufps xmm7, xmm7, 0h
	shufps xmm8, xmm8, 0h


	pand xmm0, xmm5
	pand xmm1, xmm6
	pand xmm2, xmm7
	pand xmm3, xmm8

;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx


	xor rbx, rbx ; Pongo los que serán bits 0 y 1, correspondientemente en los primeros 4 bytes
	xor r10, r10 ; Pongo los que serán bits 2 y 3, correspondientemente en los primeros 4 bytes
	xor r13, r13 ; Pongo los que serán bits 4 y 5, correspondientemente en los primeros 4 bytes
	xor r14, r14 ; Pongo los que serán bits 6 y 7, correspondientemente en los primeros 4 bytes

	mov ebx, 0xc080400
	mov r10d, 0xD090501
	mov r13d, 0xE0A0602
	mov r14d, 0xF0B0703

	movq xmm5, rbx
	movq xmm6, r10
	movq xmm7, r13
	movq xmm8, r14

	pshufb xmm0, xmm5
	pshufb xmm1, xmm6
	pshufb xmm2, xmm7
	pshufb xmm3, xmm8

	psllw xmm1, 2
	psllw xmm2, 4
	psllw xmm3, 6

	; Voy completando los bytes en xmm0
	por xmm0, xmm1
	por xmm0, xmm2
	por xmm0, xmm3

;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

	movdqu [rsi], xmm0

	add rdi, 12
	add r11, 12
	add rsi, 3
	add r9, 3

	cmp r11, r8 ; Termino si ya recorrí todos los píxeles
	jge .fin
	cmp r9, r12
	jl .ciclo



.fin:
	mov [rsi], byte 0x0 ; Para terminar el string
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rbp
    ret
