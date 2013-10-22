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
	MOVDQA xmm4, xmm1 ; Guardo los bits op en xmm4

	MOVDQA xmm2, xmm1
	MOV EBX,01010101h
	movd xmm3, ebx
	shufps xmm3, xmm3, 0h
	MOVDQA xmm5, xmm3
	pand xmm2, xmm3
	PSRLW xmm1, 1

	PAND xmm1, xmm2
	PXOR xmm1, xmm2
	; En xmm1 quedó la máscara para op = 1

	MOVDQA xmm2, xmm4 ; Guardo los bits op en xmm2
	MOVDQA xmm6, xmm4 ; Guardo los bits op en xmm6
	PSRLW xmm4, 1
	PAND xmm4, xmm5 ; Me quedo con el bit más significativo del op
	PANDN xmm2, xmm4 ; Niego los bits op y luego hago and con xmm4. Me quedan en 1 los bytes cuyo op original era 2
	; En xmm2 quedó la máscara para op = 2

	MOVDQA xmm3, xmm0
	MOVDQA xmm4, xmm0
	MOV ebx, 0b0b0b0bh
	movd xmm6, ebx
	shufps xmm6, xmm6, 0h
	MOV ebx, 03030303h
	movd xmm7, ebx
	shufps xmm7, xmm7, 0h
	MOV ebx, 0xFFFFFFFF
	movd xmm8, ebx
	shufps xmm8, xmm8, 0h

	MOV ebx, 0f0f0f0fh
	movd xmm10, ebx
	shufps xmm10, xmm10, 0h
	pand xmm0, xmm10
	movdqa xmm3, xmm0
	movdqa xmm4, xmm0

	pcmpgtb xmm0, xmm6
	pand xmm3, xmm0
	pand xmm7, xmm0
	pxor xmm0, xmm8
	pand xmm4, xmm0
	pxor xmm3, xmm7
	paddb xmm3, xmm4 ; En xmm3 queda aplicada la negacion para los op 3

	paddb xmm3, xmm1 ; Sumo 1 a los correspondientes
	psubb xmm3, xmm2 ; Resto 1 a los correspondientes

	MOV ebx, 03030303h
	movd xmm5, ebx
	shufps xmm5, xmm5, 0h
	pand xmm3, xmm5 ; Borro todos los bits que no me interesan


	MOVDQA xmm0, xmm3
	MOVDQA xmm1, xmm3
	MOVDQA xmm2, xmm3


	; Dejo en xmm0 los bits que seran 0 y 1 de cada byte
	MOV ebx, 00000003h
	movd xmm5, ebx
	shufps xmm5, xmm5, 0h
	pand xmm0, xmm5

	; Dejo en xmm1 los bits que seran 2 y 3 de cada byte
	MOV ebx, 00000300h
	movd xmm5, ebx
	shufps xmm5, xmm5, 0h
	pand xmm1, xmm5

	; Dejo en xmm2 los bits que seran 4 y 5 de cada byte
	MOV ebx, 00030000h
	movd xmm5, ebx
	shufps xmm5, xmm5, 0h
	pand xmm2, xmm5

	; Dejo en xmm3 los bits que seran 6 y 7 de cada byte
	MOV ebx, 03000000h
	movd xmm5, ebx
	shufps xmm5, xmm5, 0h
	pand xmm3, xmm5

;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

	; Pongo los que serán bits 0 y 1, correspondientemente en los primeros 4 bytes
	xor rbx, rbx
	mov ebx, 0xc080400
	movq xmm5, rbx
	pshufb xmm0, xmm5

	; Pongo los que serán bits 2 y 3, correspondientemente en los primeros 4 bytes
	xor rbx, rbx
	mov ebx, 0xD090501
	movq xmm5, rbx
	pshufb xmm1, xmm5
	psllw xmm1, 2

	; Pongo los que serán bits 4 y 5, correspondientemente en los primeros 4 bytes
	xor rbx, rbx
	mov ebx, 0xE0A0602
	movq xmm5, rbx
	pshufb xmm2, xmm5
	psllw xmm2, 4

	; Pongo los que serán bits 6 y 7, correspondientemente en los primeros 4 bytes
	xor rbx, rbx
	mov ebx, 0xF0B0703
	movq xmm5, rbx
	pshufb xmm3, xmm5
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
	pop r12
	pop rbx
	pop rbp
    ret
