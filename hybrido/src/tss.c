/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
extern unsigned int TASK_CODE_SRC_ARRAY[] ;

tss tarea_inicial;
tss tarea_idle;

tss tss_navios[CANT_TAREAS];
tss tss_banderas[CANT_TAREAS];

void tss_inicializar() {
	bleach_tss(&tarea_inicial);
	bleach_tss(&tarea_idle);
}

void bleach_tss(tss * task){
	(* task) = (tss) {
    .unused0 = 0x0,
    .unused1 = 0x0,
    .unused2 = 0x0,
    .unused3 = 0x0,
    .unused4 = 0x0,
    .unused5 = 0x0,
    .unused6 = 0x0,
    .unused7 = 0x0,
    .unused8 = 0x0,
    .unused9 = 0x0,
    .unused10 = 0x0,

    .ptl = 0x0
    ,
    .esp0 = 0x0,    
    .esp1 = 0x0,
    .esp2 = 0x0,

    .ss0 = 0x0,
    .ss1 = 0x0,
    .ss2 = 0x0,
    
    .cr3 = 0x0,

    .eip = 0x0,

    .eflags = 0x00000002, //Esto tiene que quedar en 2 o no hay interrupciones

    .eax = 0x0,
    .ecx = 0x0,
    .edx = 0x0,
    .ebx = 0x0,
    .esp = 0x0,
    .ebp = 0x0,
    .esi = 0x0,
    .edi = 0x0,

    .es = 0x0,
    .cs = 0x0,
    .ss = 0x0,
    .ds = 0x0,
	.fs = 0x0,
  
    .gs = 0x0,
    .ldt = 0x0,
   
    .dtrap = 0x0,
    .iomap = 0x0,
	};
};

void tss_tarea_inicializar(int num_task){
	tss task = tss_navios[num_task];
	tss bandera = tss_banderas[num_task];

	long unsigned int posicion_directorio = SECTORFREEMEM;
	int i = num_task;
	while( i < 0){
		posicion_directorio += (TAMANO_PAGINA);
		posicion_directorio += (TAMANO_PAGINA);
		i--;
	};

    bleach_tss(& task); bleach_tss(& bandera);
    //eip
    task.eip = POSVIRTUAL_TAREAS; bandera.eip = POSVIRTUAL_TAREAS+ TAMANO_PAGINA;                              
    //cr3
	task.cr3 = (posicion_directorio) >> 12; bandera.cr3 = (posicion_directorio + (TAMANO_PAGINA)) >> 12;
    //pila cero
    task.esp0 = PILALVLCERO ;   bandera.esp0 = PILALVLCERO ;
    //cs
    task.cs = GDT_IDX_CODE_3 * 8; bandera.cs = GDT_IDX_CODE_3  * 8;
    //ds - fs -es -gs  
    task.ds = GDT_IDX_DATA_3  * 8; bandera.ds = GDT_IDX_DATA_3   * 8;
    task.fs = GDT_IDX_DATA_3  * 8; bandera.fs = GDT_IDX_DATA_3   * 8;
    task.es = GDT_IDX_DATA_3  * 8; bandera.es = GDT_IDX_DATA_3   * 8;
    task.gs = GDT_IDX_DATA_3  * 8; bandera.gs = GDT_IDX_DATA_3   * 8;
    //ss (ss3) - ss0 - ss1 - ss2
    task.ss = GDT_IDX_DATA_3  * 8; bandera.ss = GDT_IDX_DATA_3   * 8;
    task.ss0 = GDT_IDX_DATA_3 * 8; bandera.ss0 = GDT_IDX_DATA_3  * 8;
    task.ss1 = GDT_IDX_DATA_3 * 8; bandera.ss1 = GDT_IDX_DATA_3  * 8;
    task.ss2 = GDT_IDX_DATA_3 * 8; bandera.ss2 = GDT_IDX_DATA_3  * 8;
    //EBP ESP

}