; void waves_c (
;	unsigned char *src,
;	unsigned char *dst,
;	int m,
;	int n,
;	int row_size,
;	float x_scale,
;	float y_scale,
;	float g_scale
; );

; Parámetros:
; 	rdi = src
; 	rsi = dst
; 	rdx = m
; 	rcx = n
; 	r8 = row_size
; 	xmm0 = x_scale
; 	xmm1 = y_scale
; 	xmm2 = g_scale


global waves_asm

section .text

waves_asm:
	;; TODO: Implementar


	ret
