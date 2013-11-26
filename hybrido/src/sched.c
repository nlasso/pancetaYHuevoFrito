/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/
#include "defines.h"
#include "sched.h"

#define INDICE_IDLE		0

// Indice de tareas.
#define unsigned int indices_tareas[] = {GDT_TSS_TS1, GDT_TSS_TS2, GDT_TSS_TS3, GDT_TSS_TS4, GDT_TSS_TS5, GDT_TSS_TS6, GDT_TSS_TS7, 
								GDT_TSS_TS8};

// Indice de banderas.
#define unsigned int indices_banderas[] = {GDT_TSS_FG1, GDT_TSS_FG2, GDT_TSS_FG3, GDT_TSS_FG4, GDT_TSS_FG5, GDT_TSS_FG6, GDT_TSS_FG7
								GDT_TSS_FG8};

void sched_inicializar() {
	// Carlo la tarea idle.
	struct tarea_t tarea_idle;
	tarea_idle.tarea = (GDT_TSS_IDLE << 3) + 0x03;
	tarea_idle.bandera = (GDT_TSS_IDLE << 3) + 0x03;	//Por precaucion pongo la misma tarea. pero IDLE no tiene bandera!
	tadea_idle.estado = 1;

	sched.tareas[0] = tarea_idle;

	// Cargo los selectores de segmentos en mi estructura de scheduling.
	for (int i = 1; i < CANT_TAREAS; i++)
	{
		struct tarea_t tarea_struct;
		tarea_struct.tarea = (indices_tareas[i] << 3) + 0x03;
		tarea_struct.bandera = (indices_banderas[i] << 3) + 0x03;
		tarea_struct.estado = 1;

		sched.tareas[i] = tarea_struct;
	}
	sched.QUANTUM_RESTANTE = QUANTUM_TAREA;
	sched.TAREA_ACTUAL = sched.tareas[INDICE_IDLE];
	sched.CONTEXTO = 0;
}

void desalojar_tarea(){
	unsigned int _tarea_actual = sched.TAREA_ACTUAL;
	saltar_idle(); // Antes de borrarlo salto a la tarea IDLE.

	// Desalojo Tarea y bandera, es decir los borro de sus respectivos arrays.
	indices_tareas[_tarea_actual].estado = 0;
}
void saltar_idle(){
	// Tengo que saltar a la tarea IDLE y correr hasta que se termine el quantum.
	sched.TAREA_ACTUAL = 0;
}

void restaurar_quantum(){
	sched.QUANTUM_RESTANTE = QUANTUM_TAREA;
}

void saltar_a_tarea(int indice_siguiente_tarea){
	sched.TAREA_ACTUAL = indice_siguiente_tarea;
}

unsigned int dame_tarea_actual(){
	return sched.TAREA_ACTUAL;
}

unsigned int dame_bandera(int bandera){
	return sched.tareas[bandera].bandera;
}

void cambiar_contexto(int _contexto){
	sched.CONTEXTO = _contexto;
}

unsigned short sched_proximo_indice() {
	unsigned int tarea_actual = sched.TAREA_ACTUAL;
	tarea_actual++;
	unsigned int siguiente_indice = siguiente_indice_posible(tarea_actual);
	saltar_a_tarea(siguiente_indice);			//VERIFICAR: Tengo que saltar si o si??
    return sched.tareas[siguiente_indice].tarea;
}

unsigned short siguiente_indice_posible(tarea_actual){
	if(tarea_actual == CANT_TAREAS  + 1){
		tarea_actual == INDICE_IDLE;
		return tarea_actual;
	}else{
		if(sched.tareas[tarea_actual].estado != 0){
			return tarea_actual;
		}else{
			tarea_actual++;
			return siguiente_indice_posible(tarea_actual);
		}
	}
}

unsigned short sched_proxima_bandera(){
	return 0;
}
