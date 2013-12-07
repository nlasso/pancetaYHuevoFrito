/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
extern unsigned int TASK_CODE_SRC_ARRAY[] ;
extern unsigned int TASK_CR3[] ;
extern unsigned int  TASK_PAG_2[];
extern struct sched_t sched;

tss tarea_inicial;
tss tarea_idle;

tss tss_navios[CANT_TAREAS];
tss tss_banderas[CANT_TAREAS];
int eip_bandera[] = {0x0,
                     0x0,
                     0x0,
                     0x0,
                     0x0,
                     0x0,
                     0x0,
                     0x0,
                     0x0
                     /*TASK_1_CODE_SRC_ADDR + 0x1FFC,
                     TASK_2_CODE_SRC_ADDR + 0x1FFC,
                     TASK_3_CODE_SRC_ADDR + 0x1FFC,
                     TASK_4_CODE_SRC_ADDR + 0x1FFC,
                     TASK_5_CODE_SRC_ADDR + 0x1FFC,
                     TASK_6_CODE_SRC_ADDR + 0x1FFC,
                     TASK_7_CODE_SRC_ADDR + 0x1FFC,
                     TASK_8_CODE_SRC_ADDR + 0x1FFC*/};

void tss_inicializar() {
    long unsigned int _pila3_idle = POSVIRTUAL_TAREAS + 0x1C00;
	bleach_tss(&tarea_inicial);
    definir_tss(&tarea_idle, MAINPAGEDIR, PILALVLCERO, POSVIRTUAL_TAREAS, 0, _pila3_idle);
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
    long unsigned int pos_pila3;
    while(num_task - 1 < CANT_TAREAS){
    	tss* task = &(tss_navios[num_task-1]);
    	tss* bandera = &(tss_banderas[num_task-1]);

        pos_dir = TASK_CR3[num_task];
        pos_codigo = POSVIRTUAL_TAREAS; 
        pos_pila0 = POSVIRTUAL_TAREAS + (TAMANO_PAGINA * 4);
        pos_pila3 = POSVIRTUAL_TAREAS + 0x1C00;
        definir_tss(task , pos_dir, pos_pila0, pos_codigo, 3, pos_pila3);

        pos_pila0  -= (TAMANO_PAGINA/2);
        pos_pila3 = POSVIRTUAL_TAREAS + 0x1FFC;
        pos_codigo = POSVIRTUAL_TAREAS;
        pos_codigo += (*(int *)(0x11FFC));
        definir_tss(bandera, pos_dir, pos_pila0, pos_codigo, 3, pos_pila3);


        num_task++;
    }
    //tss_fetch_eip_banderas();
    //tss_reset_flags();
}

void definir_tss(tss * task, long unsigned int _cr3, long unsigned int _esp0, long unsigned int _eip, char _priviledge,long unsigned int _pila3){
    bleach_tss(task);

    //INPUT
    (*task).cr3 = _cr3;
    (*task).eip = _eip;
    (*task).esp0 = _esp0;

    //CONSTANTES
    (*task).ss0 = GDT_IDX_DATA_0 << 3;
    (*task).esp = _pila3;//la catedra lo define asi
    (*task).ebp = (*task).esp;

    //SELECTORES
    long unsigned int DATA;
    long unsigned int CODE;

    if(_priviledge == 3){   
        DATA = (GDT_IDX_DATA_3 << 3) + 0x03; 
        CODE = (GDT_IDX_CODE_3 << 3) + 0x03;
    }else{                  
        DATA = (GDT_IDX_DATA_0 << 3); 
        CODE = (GDT_IDX_CODE_0 << 3);
    }

    (*task).cs = CODE;
    (*task).ds = DATA;
    (*task).fs = DATA;
    (*task).es = DATA;
    (*task).gs = DATA;
    (*task).ss = DATA; 
};

/*void tss_fetch_eip_banderas(){
    int contador = 1;
    while(contador <= CANT_TAREAS){
        tss_fetch_eip_singe_banderas(contador);
        //eip_bandera[tarea] = TASK_PAG_2[tarea] + 0xFFC;
       contador++;
    }
}*/

void tss_fetch_eip_flag(int tarea){
    eip_bandera[tarea] = TASK_PAG_2[tarea] + 0xFFC;
}

void tss_reset_eip_flag(int tarea){ //REVISAR
    tss* tss_actual = (tss*) (&tss_banderas[tarea-1]);
    (* tss_actual).esp = 0x40001FFC;
    (* tss_actual).ebp = 0x40001FFC;
    (* tss_actual).eip = 0x40000000;

    (* tss_actual).eip += *((int *)eip_bandera[tarea]);
}


/*void tss_reset_flags(){ //necesita schedule
    tss_reset_eip_flag(1);
    tss_reset_eip_flag(2);
    tss_reset_eip_flag(3);
    tss_reset_eip_flag(4);
    tss_reset_eip_flag(5);
    tss_reset_eip_flag(6);
    tss_reset_eip_flag(7);
    tss_reset_eip_flag(8);
}*/