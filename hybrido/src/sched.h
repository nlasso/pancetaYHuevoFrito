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

/*-----------ESTRUCTURAS DE SCHEDULING-----------*/

/*catidad de ticks de reloj del quantum*/
int QUANTUM = 3;	// El QUANTUM lo defino aca?

/*Struct de cada tarea*/
struct tarea_t
{
	unsigned short tarea;			// TSS Segment Selector para la tarea.
	unsigned short bandera;		// TSS Segment Selector para la bandera de la tarea.
	unsigned int estado;		// Estado: si el estado == 1 entonces puedo acceder a la tarea. Si estado == 0 la tarea fue desalojada.
} tarea;

/*Main struct del scheduler*/
struct sched_t
{
	struct tarea_t tareas[CANT_TAREAS + 1];			// Esto tiene que contener una tarea por cada tarea que haya incluida la IDLE.
	unsigned int QUANTUM_RESTANTE;		// QUANTUMS restantes
	unsigned int TAREA_ACTUAL;			// Indice del array tareas que esta corriendo ahora. CUIDADO: No es un seg sel. es solo un numero indice del array.
	unsigned int BANDERA_ACTUAL;
	unsigned int CONTEXTO;				//Si el contexto == 0 entonces estoy en tareas de lo contrario estoy en bandera.
} sched;


/*---------------------SERVICIOS-------------------*/
/*SCHED_INICIALIZAR: inicializa todas las estructuras requeridas por el Kernel para funcionar. array de tareas, QUANTUMS, etc...*/
void sched_inicializar();

/*DESALOJAR_TAREA: Desaloja la tarea actual y salta a la tarea IDLE.*/
void desalojar_tarea();

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

static unsigned short siguiente_indice_posible(int);

/*SCHED_PROXIMO_INDICE: devuelve el indice de la proxima tarea.*/
unsigned short sched_proximo_indice();

/*SCHED_PROXIMA_BANDERA: devuelve el indice de la proxima bandera.*/
unsigned short sched_proxima_bandera();

/* MISC */
unsigned short siguiente_indice_posible(int);

#endif	/* !__SCHED_H__ */
