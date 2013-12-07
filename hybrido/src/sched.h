/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "defines.h"
//#include "isr.h"
#include "schedule_structure.h"
//#include "tss.h"

/*-----------ESTRUCTURAS DE SCHEDULING-----------*/

/*catidad de ticks de reloj del quantum*/
int QUANTUM = 3;	// El QUANTUM lo defino aca?

/*Struct de cada tarea*/
struct tarea_t tarea;

/*Main struct del scheduler*/
struct sched_t sched;


/*---------------------SERVICIOS-------------------*/
/*SCHED_INICIALIZAR: inicializa todas las estructuras requeridas por el Kernel para funcionar. array de tareas, QUANTUMS, etc...*/
void sched_inicializar();

void desalojar_tarea();

/*SALTAR_IDLE: salta a la tarea IDLE*/
//void saltar_idle();
unsigned int sched_proximo_indice();
unsigned int sched_proxima_bandera();

unsigned short inicializar_corrida_flags();
unsigned short continuo_corrida_flags();
unsigned short continuo_corrida_tareas();
unsigned short inicializar_idle_total();
void cambiar_contexto_idle();
unsigned short revisar_no_GPF(unsigned short);
/*CLOCK: código de la interrupcion de reloj*/
unsigned short clock();

/*BANDERA: Código de la interrupcion de bandera*/
void bandera(unsigned char*);

#endif	/* !__SCHED_H__ */
