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
	//Defino las dos tablas de paginas
	pagetab_entry *pagetab1 = (pagetab_entry *) FIRSTPAGETAB;
	pagetab_entry *pagetab2 = (pagetab_entry *) SECONDPAGETAB;

	x = 0;
	while(x < CANT_ENTRADAS){
		bleach_pagedir_entry(&pagedir[x]); 
		bleach_pagetab_entry(&pagetab1[x]); 
		bleach_pagetab_entry(&pagetab2[x]); 
		x++;
	}

	//Guardo la informacion en el directorio de tabla de paginas paginas  
	unsigned char _writable = 1; 
	unsigned char _priviledge = 0;
	define_pagedir_entry(&pagedir[0], _writable, _priviledge, (long unsigned int) pagetab1);//Defino pagedir apuntando a la primera pagina
	define_pagedir_entry(&pagedir[1], _writable, _priviledge, (long unsigned int) pagetab2);//Defino pagedir apuntando a la segunda pagina

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
		unsigned long mapeo = (TAMANO_PAGINA * (x + CANT_ENTRADAS));
		define_pagetab_entry(&pagetab2[x], _writable, _priviledge, mapeo );
		x++;
	}
		
}	

void mmu_inicializar_tareas(){
	unsigned int cont = 0;
	unsigned int i = 0;
	unsigned char _writable = 0; 
	unsigned char _priviledge = 1;
	while(cont < CANT_TAREAS + 1){
		//DEFINO PAGINAS
		pagedir_entry * pgdir  = (pagedir_entry *)  LAST_MEMORY_FREE; 	
		TASK_PAG_DIR[cont] = LAST_MEMORY_FREE;
		LAST_MEMORY_FREE += TAMANO_PAGINA;
		pagetab_entry * pgtab  = (pagetab_entry *)  FIRSTPAGETAB;	//ESTAS PAGINAS YA ESTAN DEFINIDAS POR EL IDENTITY MAPPING
		pagetab_entry * pgtab2 = (pagetab_entry *)  SECONDPAGETAB; //ESTAS PAGINAS YA ESTAN DEFINIDAS POR EL IDENTITY MAPPING
		pagetab_entry * pgtab3 = (pagetab_entry *)  LAST_MEMORY_FREE;	
		LAST_MEMORY_FREE += TAMANO_PAGINA;

		//IDENTITY MAPPING YA DEFINIDO
		i = 0;
		while(i < CANT_ENTRADAS){bleach_pagedir_entry(&pgdir[i]); i++;}
		define_pagedir_entry(&pgdir[0], _writable, _priviledge, (long unsigned int) pgtab);//Defino pagedir apuntando a la primera pagina
		define_pagedir_entry(&pgdir[1], _writable, _priviledge, (long unsigned int) pgtab2);//Defino pagedir apuntando a la segunda pagina

		//PAGE TAREA MAPPING -PAGE 3-
		//Limpio todas las paginas
		i = 0;
		while(i < CANT_ENTRADAS + 1){bleach_pagetab_entry(&pgtab3[i]);i++;}
		//defino la entrada
		define_pagedir_entry(&pgdir[0x100], _writable, _priviledge, (long unsigned int) pgtab3);
		long unsigned int mapeo = TASK_CODE_SRC_ARRAY[cont];
		//long unsigned int mapeo = TASK_1_CODE_SRC_ADDR;
		define_pagetab_entry(&pgtab3[0], _writable, _priviledge, mapeo );
		define_pagetab_entry(&pgtab3[1], _writable, _priviledge, mapeo + TAMANO_PAGINA );
		define_pagetab_entry(&pgtab3[2], _writable, _priviledge, 0X0 );

		//FIN LOOP
		cont++;
	}	
}

void * free_memory(){
	void * ret = (void *) LAST_MEMORY_FREE;
	if(LAST_MEMORY_FREE == 0x9FFFF){
		char * error_text = "Nos quedamos sin free_memory en kernel";
		print_texto( &pantalla_actual,0,0, error_text ,(C_FG_WHITE | C_BG_BLACK));
		int x = 0;
		while(1){x++;} //TILDO EL SISTEMA
	}else{LAST_MEMORY_FREE += TAMANO_PAGINA;}
	return ret;

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
	    (*tabla).dirbase_12_31 = base >> 12;
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

