%define breakpoint xchg bx,bx

;;GDT
%define GDT_NULL_DESC           0
%define GDT_CODE_0				18 * 8
%define GDT_CODE_3				19 * 8
%define GDT_DATA_0				20 * 8
%define GDT_DATA_3				21 * 8
%define GDT_VIDEO				22 * 8
%define GDT_IDLE				23 * 8
%define GDT_TS1					24 * 8
%define GDT_TS1					24 * 8
%define GDT_TS2					25 * 8
%define GDT_TS3					26 * 8
%define GDT_TS4					27 * 8
%define GDT_TS5					28 * 8
%define GDT_TS6					29 * 8
%define GDT_TS7					30 * 8
%define GDT_TS8					31 * 8

; INTERRUPCIONES
%define INTCLOCK 31 	
%define INTKEYBOARD 32 	
%define INTSERVICIOS 0x50
%define ANCLA 	0x923
%define MISIL 	0x83A
%define NAVEGAR 0xAEF
%define INTBANDERA 0x66

;; PILAS
%define PILAKERNEL				0x00026000
%define PILALVLCERO	 			0x00029000 

;; PANTALLAS
%define SCREENESTADO			0
%define SCREENMAPA				1


;; PAGINAS

%define MAINPAGEDIR 			0x00027000  
%define IDLEPAGEDIR 			0x00031000  