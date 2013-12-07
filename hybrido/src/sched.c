/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/
#include "defines.h"
#include "sched.h"
#include "isr.h"

#define INDICE_IDLE 0
#define EN_IDLE_TOTAL 0
#define EN_IDLE_TAREA 1
#define EN_IDLE_FLAG 2
#define EN_TAREA 3
#define EN_FLAG 4

//extern void tss_reset_flags();

extern void load_pantalla();
extern void print_bandera(int);
extern void print_banderines();
extern void print_tablatar_error(int, char*);
//extern void tss_fetch_eip_banderas();
extern void tss_fetch_eip_flag(int);
extern void tss_reset_eip_flag(int);

// Indice de tareas.
unsigned int indices_tareas[] = {GDT_TSS_TS1, GDT_TSS_TS2, GDT_TSS_TS3, GDT_TSS_TS4, GDT_TSS_TS5, GDT_TSS_TS6, GDT_TSS_TS7, 
								GDT_TSS_TS8};

// Indice de banderas.
unsigned int indices_banderas[] = {GDT_TSS_FG1, GDT_TSS_FG2, GDT_TSS_FG3, GDT_TSS_FG4, GDT_TSS_FG5, GDT_TSS_FG6, GDT_TSS_FG7, GDT_TSS_FG8};

void sched_inicializar() {
	// Carlo la tarea idle.
	struct tarea_t tarea_idle;
	tarea_idle.tarea = (GDT_TSS_IDLE << 3) + 0x00;
	tarea_idle.bandera = (GDT_TSS_IDLE << 3) + 0x00;	//Por precaucion pongo la misma tarea. pero IDLE no tiene bandera!
	tarea_idle.estado = 1;
	sched.tareas[0] = tarea_idle;

	// Cargo los selectores de segmentos en mi estructura de scheduling.
	int i = 0;
	for (i = 0; i < CANT_TAREAS; i++) 
	{
		struct tarea_t tarea_struct;
		tarea_struct.tarea = (indices_tareas[i] << 3) + 0x03;
		tarea_struct.bandera = (indices_banderas[i] << 3) + 0x03;
		tarea_struct.estado = 1;
		tarea_struct.indice = i + 1;
		sched.tareas[i + 1] = tarea_struct;
	}
	sched.QUANTUM_RESTANTE = QUANTUM_TAREA + 1;
	sched.TAREA_ACTUAL = INDICE_IDLE;
	sched.CONTEXTO = EN_IDLE_TAREA;
	sched.BANDERA_ACTUAL = 0;
	sched.TASKS_UP = CANT_TAREAS;
	//sched.IDLE_ON = 1;
	//DESACTIVAR PARA TESTEAR
	//tss_reset_flags();
}

void desalojar_tarea_actual(){
	desalojar_tarea(sched.TAREA_ACTUAL);	
}

void desalojar_tarea(int tarea){
	sched.tareas[tarea].estado = 0;
	sched.TASKS_UP--;
}

void saltar_idle(){
	cambiar_contexto_idle();	
	print_banderines();
	//DESACTIVAR PARA TESTEAR
	jump_idle();
}

unsigned int sched_proxima_tarea(){
	int task = sched.TAREA_ACTUAL;
	int intentos = 8;
	char found = 0;
	task ++;
	while(intentos > 0){
		intentos--;
		// Si hay overflow termino
		if(task == CANT_TAREAS+ 1){ task = 1;}
		// Si es esta tarea termino de buscar
		if(sched.tareas[task].estado != 0){ intentos = 0; found = task;}
		// Su no es esta tarea siguo buscando
		else{task++;}
	}
	return found;
}

unsigned int sched_proxima_bandera(){
	int task = sched.BANDERA_ACTUAL;
	int intentos = 8;
	char found = 0;
	task ++;
	while(intentos > 0){
		intentos--;
		// Si hay overflow termino
		if(task == CANT_TAREAS + 1){ intentos = 0; found = 0;}
		// Si es esta tarea termino de buscar
		else if(sched.tareas[task].estado != 0){ intentos = 0; found = task;}
		// Su no es esta tarea siguo buscando
		else{task++;}
	}
	return found;
}

void cambiar_contexto_idle(){
	switch (sched.CONTEXTO){
		case EN_TAREA:
		case EN_IDLE_TAREA:
			sched.CONTEXTO = EN_IDLE_TAREA;
			break;
		case EN_FLAG:
		case EN_IDLE_FLAG:
			sched.CONTEXTO = EN_IDLE_FLAG;
			break;
		default:
			sched.CONTEXTO = EN_IDLE_TOTAL;
			break;
		};
}

unsigned short inicializar_idle_total(){
	unsigned short respuesta = sched.tareas[INDICE_IDLE].tarea ;
	sched.TAREA_ACTUAL   = 0;
	sched.BANDERA_ACTUAL = 0;
	sched.CONTEXTO = EN_IDLE_TOTAL;
	return respuesta;
}

unsigned short inicializar_corrida_flags(){
	sched.BANDERA_ACTUAL = 0;
	sched.BANDERA_ACTUAL = sched_proxima_bandera();
	tss_fetch_eip_flag(sched.BANDERA_ACTUAL);
	tss_reset_eip_flag(sched.BANDERA_ACTUAL);
	unsigned short respuesta = sched.tareas[sched.BANDERA_ACTUAL].bandera;
	sched.CONTEXTO = EN_FLAG;
	return respuesta;
}

unsigned short continuo_corrida_flags(){
	unsigned short respuesta;
	unsigned int NEXT_INDEX = sched_proxima_bandera();
	sched.BANDERA_ACTUAL = NEXT_INDEX;
	if(NEXT_INDEX == 0){
		// Si no quedan banderas, salto a una tarea 
		//tss_fetch_eip_bandera();
		//tss_reset_flags();//DESACTIVAR PARA TESTEAR
		sched.QUANTUM_RESTANTE = 3;
		respuesta = continuo_corrida_tareas();
	}else{
		// Si quedan banderas, salto a una tarea 
		sched.CONTEXTO = EN_FLAG;
		tss_fetch_eip_flag(sched.BANDERA_ACTUAL);
		tss_reset_eip_flag(sched.BANDERA_ACTUAL);
		respuesta = sched.tareas[sched.BANDERA_ACTUAL].bandera;
	}
	return respuesta;
}


unsigned short continuo_corrida_tareas(){
	int respuesta;
	int TEMP = sched_proxima_tarea();
	if((sched.CONTEXTO != EN_TAREA) || (TEMP != sched.TAREA_ACTUAL) ){
		sched.TAREA_ACTUAL = TEMP;
		respuesta = sched.tareas[sched.TAREA_ACTUAL].tarea;
	}else{
		respuesta = 0;
	}
	
	sched.CONTEXTO = EN_TAREA;
	return respuesta;
}

unsigned short clock(){
	// VARIABLES
	//unsigned short NEXT_INDEX;
	unsigned short direccion_salto = 0;
	//MODIFICACIONES 
	if(sched.QUANTUM_RESTANTE > 0 ){sched.QUANTUM_RESTANTE--;}
	/*CREO QUE ESTO NO HACE FALTA, LO AGREGO SOLO POR SEGURIDAD */
	if(sched.TASKS_UP == 0){sched.CONTEXTO = EN_IDLE_TOTAL;}; 
	/*															*/
	//CLOCK
	switch (sched.CONTEXTO){

		case EN_IDLE_TOTAL://Este es el idle eterno donde ya no quedan tareas
			if(sched.TAREA_ACTUAL != 0){//CREO QUE ESTO NO HACE FALTA, LO AGREGO SOLO POR SEGURIDAD
				inicializar_idle_total();
			};
			break;

		case EN_TAREA: //Aca entro si estoy en una tarea. (identico al tarea idle)
		case EN_IDLE_TAREA:	//	Aca entro siempre que una tarea termino,ya sea por que llamo a 
			//	un syscall o por que fue despejada por una interrupcion
			if(sched.QUANTUM_RESTANTE == 0){	// Si tengo que saltar a las banderas
				direccion_salto = inicializar_corrida_flags();
			}else{								// Si tengo que seguir corriendo tareas
				direccion_salto = continuo_corrida_tareas();
			}
			break;

		case EN_IDLE_FLAG:	//	Aca entro siempre que una flag termino, ya sea por que llamo a 
			//  int 66 o por que fue despejada por una interrupcion
			direccion_salto = continuo_corrida_flags();
			break;

		case EN_FLAG:	//	Aca entro siempre que entro durante un flag.
			//	Tengo que sacar la tarea.
			desalojar_tarea(sched.BANDERA_ACTUAL);
			print_tablatar_error(sched.BANDERA_ACTUAL, "Flag time-out");
			load_pantalla();
			if(sched.TASKS_UP == 0){
				direccion_salto = inicializar_idle_total();
			}else{
				direccion_salto = continuo_corrida_flags();
			};
			break;
	};
	//DESACTIVAR PARA TESTEAR
	print_banderines();
	direccion_salto = revisar_no_GPF(direccion_salto);
	return direccion_salto;
}

void bandera(){
	if(sched.CONTEXTO == EN_TAREA){
		desalojar_tarea(sched.TAREA_ACTUAL);
		//DESACTIVAR PARA TESTEAR
		//
		print_tablatar_error(sched.BANDERA_ACTUAL, "Int66 in task");
		load_pantalla();
	}else{
		print_bandera(sched.BANDERA_ACTUAL);
	}
	//DESACTIVAR PARA TESTEAR
	//print_tablatar_error(sched.BANDERA_ACTUAL, "Llamo bandera");
	//load_pantalla();
	saltar_idle();
}

unsigned short revisar_no_GPF(unsigned short dir){
	//if ((dir < 0x100000) || (dir > AREA_MAR_FIN)){
		// BORRO TAREA
		//int borrar;
		//if(sched.CONTEXTO == EN_TAREA){ borrar = sched.TAREA_ACTUAL;}
		//						else{ borrar = sched.BANDERA_ACTUAL;}
		//desalojar_tarea(borrar);
		//print_tablatar_error(sched.BANDERA_ACTUAL, "TSS corrupted");
		// CAMBIO A IDLE
		//dir = (GDT_TSS_IDLE << 3) + 0x00;
		//cambiar_contexto_idle();
	//}
	return dir;
}