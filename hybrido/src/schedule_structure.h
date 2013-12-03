/*Struct de cada tarea*/
struct tarea_t
{
    unsigned short tarea;           // TSS Segment Selector para la tarea.
    unsigned short bandera;     // TSS Segment Selector para la bandera de la tarea.
    unsigned int estado;        // Estado: si el estado == 1 entonces puedo acceder a la tarea. Si estado == 0 la tarea fue desalojada.
    unsigned int indice;
};

/*Main struct del scheduler*/
struct sched_t
{
    struct tarea_t tareas[CANT_TAREAS + 1];         // Esto tiene que contener una tarea por cada tarea que haya incluida la IDLE.
    unsigned int QUANTUM_RESTANTE;      // QUANTUMS restantes
    unsigned int TAREA_ACTUAL;          // Indice del array tareas que esta corriendo ahora. CUIDADO: No es un seg sel. es solo un numero indice del array.
    unsigned int BANDERA_ACTUAL;
    unsigned int CONTEXTO;              // Si el contexto == 0 entonces estoy en tareas de lo contrario estoy en bandera.
    unsigned int TASKS_UP;              // Numero de tareas habilitadas en ese momento. Si el numero es 0 se entiende que solo puede correr la tarea IDLE
};