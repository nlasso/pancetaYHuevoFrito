/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
extern unsigned int TASK_CODE_SRC_ARRAY[] ;
extern unsigned int TASK_PAG_DIR[] ;
extern void mmu_mapear_pagina(unsigned int, unsigned int, unsigned int);
extern void mmu_unmapear_pagina(unsigned int, unsigned int);
extern void mmu_backdoor_mapping_task(unsigned int, int);
extern void mmu_backdoor_unmapping(unsigned int, int);
extern struct sched_t sched;


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
    	tss* task = &(tss_navios[num_task-1]);
    	tss* bandera = &(tss_banderas[num_task-1]);

        pos_dir = TASK_PAG_DIR[num_task];
        pos_codigo = POSVIRTUAL_TAREAS; 
        //ESTO PUEDE ESTAR MAL
        //ESTO PUEDE ESTAR MAL
        //ESTO PUEDE ESTAR MAL
        //ESTO PUEDE ESTAR MAL
        pos_pila0 = POSVIRTUAL_TAREAS + (TAMANO_PAGINA * 4);

        definir_tss(task , pos_dir, pos_pila0, pos_codigo, 3);

        pos_codigo += TAMANO_PAGINA;
        pos_pila0  -= (TAMANO_PAGINA/2);

        definir_tss(bandera, pos_dir, pos_pila0, pos_codigo, 3);

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
    (*task).ss0 = GDT_IDX_DATA_0 << 3; 
    (*task).esp = POSVIRTUAL_TAREAS + 0X1C00;//la catedra lo define asi
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
 //   DATA = GDT_IDX_DATA_0; 
 //   CODE = GDT_IDX_CODE_0;
    //DATA *= 8; CODE *= 8;

    (*task).cs = CODE;
    (*task).ds = DATA;
    (*task).fs = DATA;
    (*task).es = DATA;
    (*task).gs = DATA;
    (*task).ss = DATA; 
};

void mmu_backdoor_mapping_tss(unsigned int cr3, int tarea){
    int backdoor = 0x40004000;
    int dir_fisica= (int) (&tss_banderas[tarea]);
    mmu_mapear_pagina(backdoor, cr3, dir_fisica);
}

void tss_reset_eip_flag(unsigned int cr3, int tarea){
    mmu_backdoor_mapping_task(cr3, tarea);
    mmu_backdoor_mapping_tss(cr3, tarea);
    tss* tss_actual = (tss*) 0x40004000;
    int* pointer_flag = (int *) (0x40007000 - 8);
    int _eip = (*pointer_flag) + 0x40000000;
    (* tss_actual).eip = _eip;
    mmu_backdoor_unmapping(cr3, 3);
}

void tss_reset_eip_task(unsigned int cr3, int tarea){
    mmu_backdoor_mapping_tss(cr3, tarea);
    tss* tss_actual = (tss*) 0x40004000;
    int _eip = 0x4000000;
    (* tss_actual).eip = _eip;
    mmu_backdoor_unmapping(cr3, 1);
}

void tss_fullreset_no_schedule(){
    int cr3 = MAINPAGEDIR;
    tss_reset_eip_flag(cr3, 1);
    tss_reset_eip_flag(cr3, 2);
    tss_reset_eip_flag(cr3, 3);
    tss_reset_eip_flag(cr3, 4);
    tss_reset_eip_flag(cr3, 5);
    tss_reset_eip_flag(cr3, 6);
    tss_reset_eip_flag(cr3, 7);
    tss_reset_eip_flag(cr3, 8);
}

void tss_fullreset_flags_schedule(){
    int tarea = sched.TAREA_ACTUAL;
    unsigned int cr3 = TASK_PAG_DIR[tarea];
    tss_reset_eip_flag(cr3, 1);
    tss_reset_eip_flag(cr3, 2);
    tss_reset_eip_flag(cr3, 3);
    tss_reset_eip_flag(cr3, 4);
    tss_reset_eip_flag(cr3, 5);
    tss_reset_eip_flag(cr3, 6);
    tss_reset_eip_flag(cr3, 7);
    tss_reset_eip_flag(cr3, 8);
}