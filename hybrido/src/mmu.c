/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "colors.h"
extern char pantalla_actual;
extern void print_texto(char *, int, int, char *, char);
extern struct sched_t sched;
extern void inicializar_pantalla_memoria();
extern void print_mapa_tarea (int);
extern void load_pantalla();
extern void unprint_mapa_tarea(int);
extern void print_missil(int);
extern void print_tablatar_tarea(int);
extern void print_tablatar_error(int, char *);

//////////////////////////////////////////////////////////////////////////////////
//																		//////////
// MAIN FUNCIONES 														//////////
// -0mf																	//////////
  ////////////////////////////////////////////////////////////////////////////////
pagedir_entry *mmu_primeraPagina() {return (pagedir_entry *) MAINPAGEDIR;}	


//Properties
unsigned int LAST_MEMORY_FREE = SECTORFREEMEM;

unsigned int TASK_CODE_SRC_ARRAY[] = {TASK_IDLE_CODE_SRC_ADDR, TASK_1_CODE_SRC_ADDR, TASK_2_CODE_SRC_ADDR, 
										TASK_3_CODE_SRC_ADDR, TASK_4_CODE_SRC_ADDR, TASK_5_CODE_SRC_ADDR, 
										TASK_6_CODE_SRC_ADDR, TASK_7_CODE_SRC_ADDR, TASK_8_CODE_SRC_ADDR};

unsigned int TASK_CR3[] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};



unsigned int TASK_PAG_1[] = { 0, 0X100000, 0X102000, 
							0X104000, 0X106000, 0X108000, 
							0X10A000, 0X10C000, 0X10E000};

unsigned int TASK_PAG_2[] = { 0, 0X101000, 0X103000, 
							0X105000, 0X107000, 0X109000, 
							0X10B000, 0X10D000, 0X10F000};

unsigned int TASK_PAG_3[] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

void mmu_inicializar() {
	mmu_identity_maping();
	mmu_inicializar_tareas();
}

	//Mappings
	
void mmu_identity_maping() {
	unsigned int x;
	//Dejo pagedir en blanco
	pagedir_entry *pagedir = (pagedir_entry *) MAINPAGEDIR;
	TASK_CR3[0] = MAINPAGEDIR;
	//Defino las dos tablas de paginas
	pagetab_entry *pagetab1 = (pagetab_entry *) FIRSTPAGETAB;
	pagetab_entry *pagetab2 = (pagetab_entry *) SECONDPAGETAB;
	pagetab_entry *pagetab3 = (pagetab_entry *) IDLEPAGETAB;

	x = 0;
	while(x < CANT_ENTRADAS){
		bleach_pagedir_entry(&pagedir[x]); 
		bleach_pagetab_entry(&pagetab1[x]); 
		bleach_pagetab_entry(&pagetab2[x]); 
		bleach_pagetab_entry(&pagetab3[x]); 
		x++;
	}

	//Guardo la informacion en el directorio de tabla de paginas paginas  
	unsigned char _writable = 1; 
	unsigned char _priviledge = 0;
	define_pagedir_entry(&pagedir[0]    , _writable, _priviledge, (long unsigned int) pagetab1);//Defino pagedir apuntando a la primera pagina
	define_pagedir_entry(&pagedir[1]    , _writable, _priviledge, (long unsigned int) pagetab2);//Defino pagedir apuntando a la segunda pagina
	define_pagedir_entry(&pagedir[0x100], _writable, _priviledge, (long unsigned int) pagetab3);//Defino pagedir apuntando a la segunda pagina

	//Defino la tabla 1
	x= 0; 
	while(x < CANT_ENTRADAS){ 
		bleach_pagetab_entry(&pagetab1[x]); 
		unsigned long mapeo = (TAMANO_PAGINA * x);
		define_pagetab_entry(&pagetab1[x], _writable, _priviledge, mapeo );
		x++;
	}

	
	//Defino la tabla 2
	x= 0;
	while(x < (CANT_IDENTITY_MAPEOS - CANT_ENTRADAS) ) { 
		bleach_pagetab_entry(&pagetab2[x]); 
		unsigned long mapeo = (TAMANO_PAGINA * (x+ 1024));
		define_pagetab_entry(&pagetab2[x], _writable, _priviledge, mapeo );
		x++;
	}
	//Defino la tabla 3 (para idle)
	//_priviledge = 0; //_priviledge de idle ???
	long unsigned int mapeo; mapeo = TASK_CODE_SRC_ARRAY[ 0 ] ;
	define_pagetab_entry(&pagetab3[0], _writable, _priviledge, mapeo);
	define_pagetab_entry(&pagetab3[1], _writable, _priviledge, mapeo + TAMANO_PAGINA);
	define_pagetab_entry(&pagetab3[3], _writable, _priviledge, PILALVLCERO);		
}	

void mmu_inicializar_tareas(){
	unsigned int cont = 1;
	unsigned int i = 0;
	unsigned char _writable; 
	unsigned char _priviledge;
	while(cont < CANT_TAREAS + 1){
		_writable = 0; 
		//DEFINO PAGINAS
		pagedir_entry * pgdir  = (pagedir_entry *)  LAST_MEMORY_FREE; 	
		TASK_CR3[cont] = LAST_MEMORY_FREE;
		LAST_MEMORY_FREE += TAMANO_PAGINA;
		pagetab_entry * pgtab  = (pagetab_entry *)  FIRSTPAGETAB;	//ESTAS PAGINAS YA ESTAN DEFINIDAS POR EL IDENTITY MAPPING
		pagetab_entry * pgtab2 = (pagetab_entry *)  SECONDPAGETAB; //ESTAS PAGINAS YA ESTAN DEFINIDAS POR EL IDENTITY MAPPING
		pagetab_entry * pgtab3 = (pagetab_entry *)  LAST_MEMORY_FREE;
		LAST_MEMORY_FREE += TAMANO_PAGINA * 2; 		//Es la pila que sigue no debería ser mas 2 ya que se mueve al 0??
		long unsigned int _pila0  = LAST_MEMORY_FREE;	
		LAST_MEMORY_FREE += TAMANO_PAGINA;

		//IDENTITY MAPPING YA DEFINIDO
		i = 1;
		while(i < CANT_ENTRADAS){
			bleach_pagedir_entry(&pgdir[i]); 
			i++;
		}
		_priviledge = 0;
		define_pagedir_entry(&pgdir[0], _writable, _priviledge, (long unsigned int) pgtab);//Defino pagedir apuntando a la primera pagina
		define_pagedir_entry(&pgdir[1], _writable, _priviledge, (long unsigned int) pgtab2);//Defino pagedir apuntando a la segunda pagina

		//PAGE TAREA MAPPING -PAGE 3-
		//Limpio todas las paginas
		i = 1;
		while(i < CANT_ENTRADAS + 1){
			bleach_pagetab_entry(&pgtab3[i]);
			i++;
		}
		//defino la entrada
		_priviledge = 1;
		_writable = 1;
		define_pagedir_entry(&pgdir[0x100], _writable, _priviledge, (long unsigned int) pgtab3);
		

		long unsigned int mapeo = TASK_CODE_SRC_ARRAY[cont];
		long unsigned int mar = TASK_PAG_1[cont];
		clonar_pagina(mapeo, mar);
		define_pagetab_entry(&pgtab3[0], _writable, _priviledge, mar );
		mar += TAMANO_PAGINA; 
		mapeo += TAMANO_PAGINA;
		clonar_pagina(mapeo, mar);
		define_pagetab_entry(&pgtab3[1], _writable, _priviledge, mar);
		//mar += TAMANO_PAGINA; 
		_writable = 0;
		define_pagetab_entry(&pgtab3[2], _writable, _priviledge, 0X0 );
		_priviledge = 0;
		_writable = 1;
		define_pagetab_entry(&pgtab3[3], _writable, _priviledge, _pila0);

		//FIN LOOP
		cont++;
	}	
}


	// BLEACHINGS DEFINES

void define_pagedir_entry(pagedir_entry * directorio, unsigned char escritura, 
	    unsigned char privilegio, unsigned long base){
	    //datos sabidos     
	    (*directorio).present = 1;
	    //datos a definir
	    (*directorio).dirbase_12_31 = base >> 12;
	    (*directorio).priviledge = privilegio;
	    (*directorio).writable = escritura;
	}

void define_pagetab_entry(pagetab_entry * tabla, unsigned char escritura,
	    unsigned char privilegio, unsigned long base){  
	    (*tabla).present = 1;       
	    //NOTA: NO SE UQE VA EN PAGE_ATTRIBUTE_INDEX NI EN GLOBAL
	    (*tabla).dirbase_12_31 = (base >> 12);
	    (*tabla).priviledge = privilegio;             
	    (*tabla).writable = escritura;
	    tlbflush();
}

// [Bleach] = pone todo en blanco 
void bleach_pagedir_entry(pagedir_entry * directorio){  //setea todo en 0
    (*directorio).present = 0;       (*directorio).writable = 0;
    (*directorio).priviledge = 0;    (*directorio).write_through = 0;
    (*directorio).cache_disable = 0; (*directorio).accessed = 0;
    (*directorio).ignored = 0;       (*directorio).page_size = 0;
    (*directorio).global = 0;        (*directorio).disponible_9_11 = 0x0;
    (*directorio).dirbase_12_31 = 0x00000;
}

void bleach_pagetab_entry(pagetab_entry * tabla){// setea todo en 0 
    (*tabla).present = 0;       (*tabla).writable = 0;
    (*tabla).priviledge = 0;    (*tabla).write_through = 0;
    (*tabla).cache_disable = 0; (*tabla).accessed = 0;
    (*tabla).dirty = 0;         (*tabla).page_attribute_index = 0;
    (*tabla).global = 0;        (*tabla).disponible_9_11 = 0x0;
    (*tabla).dirbase_12_31 = 0x00000;
}

pagetab_entry * get_descriptor(unsigned int virtual, unsigned int cr3){
	long unsigned int virtual_22_31 = virtual;
	virtual_22_31 = virtual_22_31 >> 22;
	long unsigned int virtual_12_21 = virtual;
	virtual_12_21 = (virtual >> 12) & 0x3FF;

	cr3 = (cr3 & 0xFFFFF000);
	pagedir_entry * directorio = (pagedir_entry *) cr3;
	// CONSIGO LA TABLA	
	long unsigned int posicion_dir = ((directorio[virtual_22_31].dirbase_12_31) << 12);
	pagetab_entry * tabla = (pagetab_entry *) posicion_dir ;
	//fisica = (long unsigned int) tabla;

	// CONSIGO EL DESCRIPTOR
	return (& tabla[virtual_12_21]);
};

void mmu_mapear_pagina(unsigned int virtual, unsigned int cr3, unsigned int fisica, unsigned char _writable){ 
	unsigned char _priviledge = 1;
	pagetab_entry * descriptor = get_descriptor(virtual, cr3);
	define_pagetab_entry(descriptor, _writable, _priviledge, fisica);
};


void mmu_unmapear_pagina(unsigned int virtual, unsigned int cr3){
	pagetab_entry * descriptor = get_descriptor(virtual, cr3);
	bleach_pagetab_entry(descriptor);
};

void canionear(unsigned int posicion_apuntada, unsigned int* buffer){//DONE
	int tarea_actual = sched.TAREA_ACTUAL;
	if((posicion_apuntada >= 0x100000)&& 
		(posicion_apuntada <= AREA_MAR_FIN) ){//varialbes
		print_missil(posicion_apuntada);
		unsigned int* destino = (unsigned int*) posicion_apuntada;
		int i = 0;
		while(i<97){destino[i] = buffer[i]; i++;}
	}else{
		desalojar_tarea(tarea_actual);
		print_tablatar_error(tarea_actual, "Invalid Canion");
	}
	load_pantalla();
}

void anclar(unsigned int destino){//DONE
	int tarea = sched.TAREA_ACTUAL;
	if(destino < 0x100000){
		int cr3 = TASK_CR3[tarea];
		unprint_mapa_tarea(tarea);
		TASK_PAG_3[tarea] = destino;
		mmu_mapear_pagina(0x40002000, cr3, destino, 0);
		print_mapa_tarea(tarea);
		print_tablatar_tarea(tarea);
	}else{
		desalojar_tarea(tarea);
		print_tablatar_error(tarea, "Invalid Ancla");
	}
	load_pantalla();
}

void navegar(unsigned int destino1, unsigned int destino2){ //DONE
	int tarea_actual = sched.TAREA_ACTUAL;

	int origen1 = TASK_PAG_1[tarea_actual];
	int origen2 = TASK_PAG_2[tarea_actual];

	if((destino1 >= 0x100000) && 
		(destino2 >= 0x100000) && 
		(destino1 <= AREA_MAR_FIN) && 
		(destino2 <= AREA_MAR_FIN))
	{
		int cr3 = TASK_CR3[tarea_actual];
		cr3 = cr3;
		unprint_mapa_tarea(tarea_actual);
		
		clonar_pagina(origen1, destino1);
		clonar_pagina(origen2, destino2);

		TASK_PAG_1[tarea_actual] = destino1;
		TASK_PAG_2[tarea_actual] = destino2;


		print_mapa_tarea(tarea_actual);
		print_tablatar_tarea(tarea_actual);

		mmu_mapear_pagina(0x40000000, cr3, destino1, 1);
		mmu_mapear_pagina(0x40001000, cr3, destino2, 1);
	}else{
		desalojar_tarea(tarea_actual);
		print_tablatar_error(tarea_actual, "Invalid Navegar");
	}	

	load_pantalla();
}

void clonar_pagina(unsigned int origen, unsigned int destino){ //DONE
	int* pag_origen = (int*)(origen);
	int* pag_destino= (int*)(destino);
	int i = 0;
	while(i < 1024){
		pag_destino[i] = pag_origen[i]; 
		i++;
	}
}