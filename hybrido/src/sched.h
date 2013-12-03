/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "defines.h"
#include "isr.h"
#include "schedule_structure.h"
#include "tss.h"

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

void desalojar_bandera();

/*SALTAR_IDLE: salta a la tarea IDLE*/
void saltar_idle();

/*RESTAURAR_QUANTUM: Resetea el quantum restante de la tarea.*/
void restaurar_quantum();

/*RESTAR_QUANTUM: resta 1 al valor del quantum*/
void restar_quantum();

/*SALTAR_A_TAREA: salta a la tarea pasada por parametro
PARAMETRO: indice del arreglo de tareas.*/
void saltar_a_tarea(int);

/*SALTAR_A_BANDERA: salta a la bandera pasada por parametro*/
void saltar_a_bandera(int);

/*DAME_TAREA_ACTUAL: devuelve la tarea que esta corriendo actualmente.*/
unsigned int dame_tarea_actual();

/*DAME_TAREA: devuelve el segment selector del indice de la tarea pasada por parametro*/
unsigned int dame_tarea(int);

/*devuelve el segment selector de la bandera.*/
unsigned int dame_bandera(int);

/*SETEAR_CONTEXTO setea el contexto dependiendo que voy a correr.*/
void cambiar_contexto(int);

static unsigned int siguiente_indice_posible(int);

/*SCHED_PROXIMO_INDICE: devuelve el indice de la proxima tarea.*/
unsigned int sched_proximo_indice();

/*SCHED_PROXIMA_BANDERA: devuelve el indice de la proxima bandera.*/
unsigned int sched_proxima_bandera();

/*CLOCK: código de la interrupcion de reloj*/
unsigned short clock();

/*BANDERA: Código de la interrupcion de bandera*/
void bandera();

#endif	/* !__SCHED_H__ */
