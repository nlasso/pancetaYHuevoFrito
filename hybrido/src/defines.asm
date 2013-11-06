%define breakpoint xchg bx,bx

;;GDT
%define GDT_NULL_DESC           0
%define GDT_CODE_0				18 * 8
%define GDT_CODE_3				19 * 8
%define GDT_DATA_0				20 * 8
%define GDT_DATA_3				21 * 8
%define GDT_VIDEO				22 * 8
%define GDT_INICIAL				23 * 8
%define GDT_IDLE				24 * 8
%define GDT_TS1					25 * 8
%define GDT_FG1					26 * 8
%define GDT_TS2					27 * 8
%define GDT_FG2					28 * 8
%define GDT_TS3					29 * 8
%define GDT_FG3					30 * 8
%define GDT_TS4					31 * 8
%define GDT_FG4					32 * 8
%define GDT_TS5					33 * 8
%define GDT_FG5					34 * 8
%define GDT_TS6					35 * 8
%define GDT_FG6					36 * 8
%define GDT_TS7					37 * 8
%define GDT_FG7					38 * 8
%define GDT_TS8					39 * 8
%define GDT_FG8					40 * 8

; INTERRUPCIONES
%define INTCLOCK 32 	
%define INTKEYBOARD 33
%define INTSERVICIOS 0x50
%define ANCLA 	0x923
%define MISIL 	0x83A
%define NAVEGAR 0xAEF
%define INTBANDERA 0x66

;; PILAS
%define PILAKERNEL				0x00026000
%define PILALVLCERO	 			0x00029000 

;; PANTALLAS
%define SCREENESTADO			1
%define SCREENMAPA				0


;; PAGINAS

%define MAINPAGEDIR 			0x00027000  
;%define IDLEPAGEDIR 			0x00031000  

%define POSVIRTUAL_TAREAS		0x40000000