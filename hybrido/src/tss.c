/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
extern unsigned int TASK_CODE_SRC_ARRAY[] ;
extern unsigned int TASK_PAG_DIR[] ;

tss tarea_inicial;
tss tarea_idle;

tss tss_navios[CANT_TAREAS];
tss tss_banderas[CANT_TAREAS];

void tss_inicializar() {
	bleach_tss(&tarea_inicial);
    definir_tss(&tarea_idle, MAINPAGEDIR, PILALVLCERO, POSVIRTUAL_TAREAS, 0);
    tss_tareas_inicializar();
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

    .eflags = 0x00000202, //Esto tiene que quedar en 202 o no hay interrupciones

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

void tss_tareas_inicializar(){
    int num_task = 1;
    long unsigned int pos_dir;
    long unsigned int pos_codigo; 
    long unsigned int pos_pila0;
    while(num_task - 1 < CANT_TAREAS){
    	tss task = tss_navios[num_task-1];
    	tss bandera = tss_banderas[num_task-1];

        pos_dir = TASK_PAG_DIR[num_task];
        pos_codigo = POSVIRTUAL_TAREAS; 
        pos_pila0 = POSVIRTUAL_TAREAS + (TAMANO_PAGINA * 3);

        definir_tss(  &task , pos_dir, pos_pila0, pos_codigo, 3);

        pos_codigo += TAMANO_PAGINA;
        pos_pila0  += (TAMANO_PAGINA/2);

        definir_tss(&bandera, pos_dir, pos_pila0, pos_codigo, 3);

        num_task++;
    }
}

void definir_tss(tss * task, long unsigned int _cr3, long unsigned int _esp0, long unsigned int _eip, char _priviledge){
    bleach_tss(task);

    //INPUT
    (*task).cr3 = _cr3;
    (*task).eip = _eip;
    (*task).esp0 = _esp0;

    //CONSTANTES
    (*task).ss0 = GDT_IDX_DATA_0 * 8;
    (*task).esp = POSVIRTUAL_TAREAS + 0X1C00;//la catedra lo define asi
    (*task).ebp = (*task).esp;

    //SELECTORES
    long unsigned int DATA;
    long unsigned int CODE;

    if(_priviledge == 3){   DATA = GDT_IDX_DATA_3; CODE = GDT_IDX_CODE_3;
    }else{                  DATA = GDT_IDX_DATA_0; CODE = GDT_IDX_CODE_0;}
 //   DATA = GDT_IDX_DATA_0; 
 //   CODE = GDT_IDX_CODE_0;
    DATA *= 8; CODE *= 8;

    (*task).cs = CODE;
    (*task).ds = DATA;
    (*task).fs = DATA;
    (*task).es = DATA;
    (*task).gs = DATA;
    (*task).ss = DATA; 
};