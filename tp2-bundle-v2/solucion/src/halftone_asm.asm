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
; 	rdx = h
; 	rcx = w
; 	r8 = src_row_size
; 	r9 = dst_row_size


global halftone_asm

section .text

halftone_asm:
	push rbp
	mov rbp, rsp
  push r12
  

  movups xmm0, [rdi] ; 15-0
  movups xmm1, [rdi+16] ; 31-16
  movq xmm2, xmm0 ; * | 7-0
  psllq xmm2, 1 ; 7-0 | *
  movq xmm3, xmm0 ; 15-0
  psrlq xmm3, 1 ; * | 15-8
  
  pop r12
  pop rbp
	ret
