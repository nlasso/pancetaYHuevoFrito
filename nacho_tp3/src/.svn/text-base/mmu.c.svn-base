/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"


//////////////////////////////////////////////////////////////////////////////////
//																		//////////
// MAIN FUNCIONES 														//////////
// -0mf																	//////////
  ////////////////////////////////////////////////////////////////////////////////
pagedir_entry *mmu_primeraPagina() {return (pagedir_entry *) MAINPAGEDIR;}	


long unsigned  int dir_codigo_tarea(int tarea){
	long unsigned int ret;
	switch (tarea){
		case 1:
	 		ret = TASK_1_CODE_SRC_ADDR;
	 		break;
	 	case 2:
	 		ret = TASK_2_CODE_SRC_ADDR;
	 		break;
	 	case 3:
	 		ret = TASK_3_CODE_SRC_ADDR;
	 		break;
	 	case 4:
	 		ret = TASK_4_CODE_SRC_ADDR;
	 		break;
	 	case 5:
	 		ret = TASK_5_CODE_SRC_ADDR;
	 		break;
	 	case 6:
	 		ret = TASK_6_CODE_SRC_ADDR;
	 		break;
	 	case 7:
	 		ret = TASK_7_CODE_SRC_ADDR;
	 		break;
	 	case 8:
	 		ret = TASK_8_CODE_SRC_ADDR;
	 		break;
	 	}
	 return ret;
}

void mmu_inicializar() {
	pagedir_entry * mpgdir = (pagedir_entry *) MAINPAGEDIR;
	pagetab_entry *pgtab = (pagetab_entry *) FIRSTPAGETAB;
	pagetab_entry *pgtab2 = (pagetab_entry *) FIRSTPAGETAB + TAMANO_PAGINA;
	mmu_identity_maping(mpgdir, pgtab, pgtab2);
	long unsigned int PGS = SECTORPAGES;
	unsigned int cont = 0;
	unsigned int i = 0;
	unsigned char _writable = 0; 
	unsigned char _priviledge = 1;
	while(cont < CANT_TAREAS){
		//DEFINO PAGINAS
		pagedir_entry * pgdir = (pagedir_entry *)  PGS; 	PGS += TAMANO_PAGINA;
		pagetab_entry *pgtab  = (pagetab_entry *)  PGS; 	PGS += TAMANO_PAGINA;
		pagetab_entry *pgtab2 = (pagetab_entry *)  PGS;		PGS += TAMANO_PAGINA;
		pagetab_entry *pgtab3 = (pagetab_entry *)  PGS;		PGS += TAMANO_PAGINA;

		//IDENTITY MAPPING -PAGE 1+2-
		mmu_identity_maping(pgdir, pgtab, pgtab2);  

		//PAGE TAREA MAPPING -PAGE 3-
		i = 0;
		while(i < CANT_ENTRADAS){bleach_pagetab_entry(&pgtab3[i]);i++;}
		define_pagedir_entry(&pgdir[0x100], _writable, _priviledge, (long unsigned int) pgtab3);
		long unsigned int mapeo = dir_codigo_tarea(cont+1);
		//long unsigned int mapeo = TASK_1_CODE_SRC_ADDR;
		define_pagetab_entry(&pgtab3[0], _writable, _priviledge, mapeo );
		define_pagetab_entry(&pgtab3[1], _writable, _priviledge, mapeo + TAMANO_PAGINA );
		define_pagetab_entry(&pgtab3[2], _writable, _priviledge, 0X0 );

		//FIN LOOP
		cont++;
	}	
}
//////////////////////////////////////////////////////////////////////////////////
//																		//////////
// DEFINICIONES 														//////////
// -1df																	//////////
  ////////////////////////////////////////////////////////////////////////////////

	//Mappings
	
void mmu_identity_maping(pagedir_entry *  pagedir , pagetab_entry * pgt, pagetab_entry * pgt2) {
	unsigned int x;
	//Dejo pagedir en blanco
	x = 0;
	while(x < CANT_ENTRADAS){
		bleach_pagedir_entry(&pagedir[x]); 
		bleach_pagetab_entry(&pgt[x]); 
		bleach_pagetab_entry(&pgt2[x]); 
		x++;
	}
	//Defino las dos tablas de paginas
	pagetab_entry *pagetab1 = pgt;
	pagetab_entry *pagetab2 = pgt2;
	//Guardo la informacion en el directorio de tabla de paginas paginas  
	unsigned char _writable = 1; 
	unsigned char _priviledge = 0;
	define_pagedir_entry(&pagedir[0], _writable, _priviledge, (long unsigned int) pgt);//Defino pagedir apuntando a la primera pagina
	define_pagedir_entry(&pagedir[1], _writable, _priviledge, (long unsigned int) pgt2);//Defino pagedir apuntando a la segunda pagina
	//SARASA
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
	//NOTA: NO ESTOY ANDA SEGURO DE ESTE NUMERO, CUIDADO!!!! 
	//while(x < (CANT_IDENTITY_MAPEOS - CANT_ENTRADAS) ) { 
	while(x < (CANT_IDENTITY_MAPEOS - CANT_ENTRADAS) ) { 
		bleach_pagetab_entry(&pagetab2[x]); 
		unsigned long mapeo = (TAMANO_PAGINA * (x + CANT_ENTRADAS));
		define_pagetab_entry(&pagetab2[x], _writable, _priviledge, mapeo );
		x++;
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

