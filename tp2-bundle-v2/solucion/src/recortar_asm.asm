; void recortar_asm (
; 	unsigned char *src,
; 	unsigned char *dst,
; 	int m,
; 	int n,
; 	int src_row_size,
; 	int dst_row_size,
; 	int tam
; );

; Parámetros:
; 	rdi = src
; 	rsi = dst
; 	rdx = m
; 	rcx = n
; 	r8 = src_row_size
; 	r9 = dst_row_size
; 	rbp + 16 = tam


global recortar_asm

section .text

recortar_asm:
	;; TODO: Implementar


	ret
