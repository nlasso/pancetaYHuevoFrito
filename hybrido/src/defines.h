/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Bool */
/* -------------------------------------------------------------------------- */
#define TRUE                    0x00000001
#define FALSE                   0x00000000


/* Graficos */
/* -------------------------------------------------------------------------- */
#define SCREENESTADO			0
#define SCREENMAPA				1
#define ASCII_first_num			48
#define ASCII_first_let			65
#define ASCII_space				0x20

/* Misc */
/* -------------------------------------------------------------------------- */
#define CANT_TAREAS             8
#define CANT_PAG_TAREA			3 // 2 DE CODIGO, 1 DE PILA
#define TAMANO_PAGINA           0x00001000
#define CANT_ENTRADAS          	0x00000400
#define CANT_IDENTITY_MAPEOS	0x00000780 //ESTO PODRIA SER 7FF, REVISAR
#define TASK_SIZE               2 * 4096

/* SCHED */
/* -------------------------------------------------------------------------- */
#define QUANTUM_TAREA			2

/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_IDX_NULL_DESC           0
#define GDT_IDX_CODE_0				18
#define GDT_IDX_CODE_3				19
#define GDT_IDX_DATA_0				20
#define GDT_IDX_DATA_3				21
#define GDT_IDX_VIDEO				22
#define GDT_TSS_INICIAL				23
#define GDT_TSS_IDLE				24
#define GDT_TSS_TS1					25
#define GDT_TSS_FG1					26
#define GDT_TSS_TS2					27
#define GDT_TSS_FG2					28
#define GDT_TSS_TS3					29
#define GDT_TSS_FG3					30
#define GDT_TSS_TS4					31
#define GDT_TSS_FG4					32
#define GDT_TSS_TS5					33
#define GDT_TSS_FG5					34
#define GDT_TSS_TS6					35
#define GDT_TSS_FG6					36
#define GDT_TSS_TS7					37
#define GDT_TSS_FG7					38
#define GDT_TSS_TS8					39
#define GDT_TSS_FG8					40

/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define BOOTSECTOR              0x00001000 /* direccion fisica de comienzo del bootsector (copiado) */
#define KERNEL                  0x00001200 /* direccion fisica de comienzo del kernel */
#define VIDEO                   0x000B8000 /* direccion fisica del buffer de video */
#define MEMVIDEO				0x000A0000

#define PILAKERNEL				0X00026000
#define PILALVLCERO	 			0x00029000  

#define BUFFERESTADO			0X0002E000
#define BUFFERMAPA				0X0002F000

#define MAINPAGEDIR 			0x00027000   /* direccion fisica del primer page dir */
#define FIRSTPAGETAB 			0x00028000   /* direccion fisica del primer page tab */
#define SECONDPAGETAB 			0x00030000   /* direccion fisica del segundo page tab */
#define IDLEPAGETAB 			0x00031000   /* direccion fisica del tercer page tab */

#define SECTORFREEMEM 			0x00032000   /* direccion fisica del primer page dir */


#define POSVIRTUAL_TAREAS		0X40000000


/* Direcciones virtuales de código, pila y datos */
/* -------------------------------------------------------------------------- */
#define TASK_CODE               0x40000000 /* direccion virtual del codigo */
#define TASK_CODE2				0x40001000 /* direccion virtual de la segunda pagina de codigo*/

//#define TASK_IDLE_CODE          0x40000000 /* direccion virtual del codigo de la tarea idle */
//#define TASK_IDLE_STACK         0x003D0000 /* direccion virtual de la pila de la tarea idle */

#define TASK_ANCLA              0x40002000
#define TASK_ANCLA_FIS          0x00000000

#define AREA_TIERRA_INICIO      0x00000000  /* 0.0 MB     */
#define AREA_TIERRA_FIN         0x000FFFFF  /* 1.0 MB - 1 */
#define AREA_MAR_INICIO         0x00100000  /* 1.0 MB     */
#define AREA_MAR_FIN            0x0077FFFF  /* 7.5 MB - 1 */

/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */
#define TASK_1_CODE_SRC_ADDR    0x00010000
#define TASK_2_CODE_SRC_ADDR    0x00012000
#define TASK_3_CODE_SRC_ADDR    0x00014000
#define TASK_4_CODE_SRC_ADDR    0x00016000
#define TASK_5_CODE_SRC_ADDR    0x00018000
#define TASK_6_CODE_SRC_ADDR    0x0001A000
#define TASK_7_CODE_SRC_ADDR    0x0001C000
#define TASK_8_CODE_SRC_ADDR    0x0001E000
#define TASK_IDLE_CODE_SRC_ADDR 0x00020000

/* INTERRUPCIONES */
#define INTCLOCK 32 	//NOTA: ESTO ESTA MAL !!!!!!!  	/ NICO: Segun las diapos el clock va a la 32.
#define INTKEYBOARD 33 	//NOTA: ESTO ESTA MAL !!!!!!!	/ NICO: Segun las diapos el teclado va a la 33.
#define INTSERVICIOS 0x50
	 //Definiciones de int servicios 
	#define ANCLA 	0x923
	#define MISIL 	0x83A
	#define NAVEGAR 0xAEF
#define INTBANDERA 0x66
////

#endif  /* !__DEFINES_H__ */
