#include <stdio.h>
#include "../sched.c"
#include "../sched.h"

extern struct sched_t sched;

char* s0 = "IDLE";
char* s3 = "TAREA";
char* s4 = "FLAG";

int nprint;

char * stringer(int i){
	char * answer;
	switch(i){
		case 0:
		case 1:
		case 2:
			answer = s0;
			break;

		case 3:
			answer = s3;
			break;
		case 4:
			answer = s4;
			break;
	}
	return answer;
}

void print_scheduler(){
	printf("+++++++++++++++++ Sched print n° %i +++++++++++++++++ \n",nprint);
	printf("Estado = %s | Tarea actual = %i | Flag actual = %i \n", stringer(sched.CONTEXTO), sched.TAREA_ACTUAL, sched.BANDERA_ACTUAL);
	printf("TAREAS [%i] [%i] [%i] [%i] [%i] [%i] [%i] [%i]\n", sched.tareas[1].estado , sched.tareas[2].estado
			, sched.tareas[3].estado , sched.tareas[4].estado , sched.tareas[5].estado , sched.tareas[6].estado
			, sched.tareas[7].estado , sched.tareas[8].estado );
	nprint ++;
}

void flag_valido(){	if(sched.CONTEXTO == EN_FLAG) sched.CONTEXTO = EN_IDLE_FLAG;}


 int main() 
  {
  	nprint = 0;
  	sched_inicializar();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    //Primera interrupcion flag
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    // Fin primera interrupcion flag
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    // Comienzo segunda interrupcion
    //flag_valido();
    clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
    flag_valido();clock();
    print_scheduler();
  } 