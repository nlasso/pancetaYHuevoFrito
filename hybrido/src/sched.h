/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

/*-----------ESTRUCTURAS DE SCHEDULING-----------*/

/*Struct de cada tarea*/
typedef struct tarea_t
{
	unsigned int tarea;			// TSS Segment Selector para la tarea.
	unsigned int bandera;		// TSS Segment Selector para la bandera de la tarea.
	unsigned int estado;		// Estado: si el estado == 1 entonces puedo acceder a la tarea. Si estado == 0 la tarea fue desalojada.
	unsigned int numero;
} task;

/*Main struct del scheduler*/
typedef struct sched_t
{
	task tareas[8];			// Esto tiene que contener una tarea por cada tarea que haya incluida la IDLE.
	unsigned int QUANTUM_RESTANTE;		// QUANTUMS restantes
	unsigned int TAREA_ACTUAL;			// Indice del array tareas que esta corriendo ahora. CUIDADO: No es un seg sel. es solo un numero indice del array.
	unsigned int CONTEXTO;				//Si el contexto == 0 entonces estoy en tareas de lo contrario estoy en bandera.
} scheduler;

/*catidad de ticks de reloj del quantum*/
int QUANTUM = 3;	// El QUANTUM lo defino aca?

scheduler sched;



/*---------------------SERVICIOS-------------------*/
/*SCHED_INICIALIZAR: inicializa todas las estructuras requeridas por el Kernel para funcionar. array de tareas, QUANTUMS, etc...*/
void sched_inicializar();

/*DESALOJAR_TAREA: Desaloja la tarea actual y salta a la tarea IDLE.*/
void desalojar_tarea();

/*SALTAR_IDLE: salta a la tarea IDLE*/
void saltar_idle();

/*RESTAURAR_QUANTUM: Resetea el quantum restante de la tarea.*/
void restaurar_quantum();

/*SALTAR_A_TAREA: salta a la tarea pasada por parametro
PARAMETRO: indice del arreglo de tareas.*/
void saltar_a_tarea(int);

/*DAME_TAREA_ACTUAL: devuelve la tarea que esta corriendo actualmente.*/
unsigned int dame_tarea_actual();

/*devuelve el segment selector de la bandera.*/
unsigned int dame_bandera(int);

/*SETEAR_CONTEXTO setea el contexto dependiendo que voy a correr.*/
void cambiar_contexto(int);

/*SCHED_PROXIMO_INDICE: devuelve el indice de la proxima tarea.*/
unsigned short sched_proximo_indice();

/*SCHED_PROXIMA_BANDERA: devuelve el indice de la proxima bandera.*/
unsigned short sched_proxima_bandera();


/* MISC */
unsigned short siguiente_indice_posible(int);

#endif	/* !__SCHED_H__ */
