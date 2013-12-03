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
extern void print_mapa_from_gdt (int, int);
extern void load_pantalla();
extern void unprint_pg_mapa_from_gdt(int,int);

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
unsigned int TASK_CODE_MAR_SRC_ARRAY[] = { 0, 0X100000, 0X102000, 
										0X104000, 0X106000, 0X108000, 
										0X10A000, 0X10C000, 0X10E000};

unsigned int TASK_PAG_DIR[] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

void mmu_inicializar() {
	mmu_identity_maping();
	mmu_inicializar_tareas();
}

	//Mappings
	
void mmu_identity_maping() {
	unsigned int x;
	//Dejo pagedir en blanco
	pagedir_entry *pagedir = (pagedir_entry *) MAINPAGEDIR;
	TASK_PAG_DIR[0] = MAINPAGEDIR;
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
		TASK_PAG_DIR[cont] = LAST_MEMORY_FREE;
		LAST_MEMORY_FREE += TAMANO_PAGINA;
		pagetab_entry * pgtab  = (pagetab_entry *)  FIRSTPAGETAB;	//ESTAS PAGINAS YA ESTAN DEFINIDAS POR EL IDENTITY MAPPING
		pagetab_entry * pgtab2 = (pagetab_entry *)  SECONDPAGETAB; //ESTAS PAGINAS YA ESTAN DEFINIDAS POR EL IDENTITY MAPPING
		pagetab_entry * pgtab3 = (pagetab_entry *)  LAST_MEMORY_FREE;
		LAST_MEMORY_FREE += TAMANO_PAGINA * 2; 		//Es la pila que sigue no debería ser mas 2 ya que se mueve al 0??
		long unsigned int _pila0  = LAST_MEMORY_FREE;	
		//LAST_MEMORY_FREE += TAMANO_PAGINA;

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
		long unsigned int mar = TASK_CODE_MAR_SRC_ARRAY[cont];
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

int get_pagina_fisica(int cr3, int dir_virtual){ //SOLO SE PUEDE USAR DESPUES DE INICIALIZAR
	pagetab_entry * descriptor = get_descriptor (dir_virtual, cr3);
	int answer = (* descriptor).dirbase_12_31 << 12;
	return answer;
}

int get_pagina_fisica_tarea(int tarea, int pagina){ //SOLO SE PUEDE USAR DESPUES DE INICIALIZAR
	unsigned int cr3 = TASK_PAG_DIR[tarea];
	unsigned int virtual = 0x40000000 + (pagina*0x1000);
	return get_pagina_fisica(cr3, virtual);
}

void mmu_mapear_pagina(unsigned int virtual, unsigned int cr3, unsigned int fisica){
	unsigned char _writable = 0; 
	unsigned char _priviledge = 1;
	pagetab_entry * descriptor = get_descriptor(virtual, cr3);
	define_pagetab_entry(descriptor, _writable, _priviledge, fisica);
};


void mmu_unmapear_pagina(unsigned int virtual, unsigned int cr3){
	pagetab_entry * descriptor = get_descriptor(virtual, cr3);
	bleach_pagetab_entry(descriptor);
};

void canionear(unsigned int posicion_apuntada, unsigned int* buffer){
	//varialbes
	int tarea = sched.TAREA_ACTUAL;
	unsigned int cr3 = TASK_PAG_DIR[tarea];
	int temp= 0x40004000;
	//
	mmu_mapear_pagina(temp,cr3, posicion_apuntada); 
	unsigned int* destino = (unsigned int*) temp;
	int i = 0;
	while(i<97){destino[i] = buffer[i]; i++;}
	// hago que el descriptor apunte a la copia
	mmu_unmapear_pagina(temp, cr3);

	
	load_pantalla();

	//inicializar_pantalla_memoria();	
}

void anclar(unsigned int destino){
	int tarea = sched.TAREA_ACTUAL;
	unsigned int cr3 = TASK_PAG_DIR[tarea];
	int temp =  0x40002000;
	mmu_mapear_pagina(temp,cr3, destino); 

}

void navegar(unsigned int destino1, unsigned int destino2){
	if((destino1 > 0x100000) && (destino2 > 0x100000)){
		int tarea_actual = sched.TAREA_ACTUAL;
		reubicar_pagina(tarea_actual, 0, destino1);
		reubicar_pagina(tarea_actual, 1, destino2);
	}		
}

void reubicar_pagina(unsigned int tarea, unsigned int numero_pagina, unsigned int destino){
	//varialbes

	unsigned int cr3 = TASK_PAG_DIR[tarea];
	int pg =  0x40000000 + 0x1000 * numero_pagina; 
	int temp= 0x40004000;
	// Borro lo que esta impreso en pantalla
	unprint_pg_mapa_from_gdt(tarea, 0);
	unprint_pg_mapa_from_gdt(tarea, 1);
	unprint_pg_mapa_from_gdt(tarea, 2);
	// preparo el descriptor dummy para la copia
	mmu_mapear_pagina(		temp,cr3, destino); 
	// copio la informacion
	clonar_pagina(pg, temp);	
	// hago que el descriptor apunte a la copia
	mmu_mapear_pagina(pg, cr3, destino);
	mmu_unmapear_pagina(temp, cr3);

	print_mapa_from_gdt(tarea,numero_pagina);
	
	load_pantalla();
	//inicializar_pantalla_memoria();	
}

void clonar_pagina(unsigned int origen, unsigned int destino){
	int* pag_origen = (int*)(origen);
	int* pag_destino= (int*)(destino);
	int i = 0;
	while(i < 1024){
		//(*pag_dest).line[i] = (*pag_orig).line[i]; 
		pag_destino[i] = pag_origen[i]; 
		i++;}
}
