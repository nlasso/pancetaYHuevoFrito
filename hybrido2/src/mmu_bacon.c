/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "defines.h"

//Properties
unsigned int LAST_MEMORY_FREE = 0x30000;
unsigned int TASK_CODE_SRC_ARRAY[] = {TASK_1_CODE_SRC_ADDR, TASK_2_CODE_SRC_ADDR, TAKS_3_CODE_SRC_ADDR, TASK_4_CODE_SRC_ADDR, TASK_5_CODE_SRC_ADDR, TASK_6_CODE_SRC_ADDR, TASK_7_CODE_SRC_ADDR, TASK_8_CODE_SRC_ADDR};
//End properties


pd_entry* pd = (pd_entry*) 0x27000;		//Tiene que ser 0x27000 por el enunciado
pt_entry* pt = (pt_entry*) 0x28000;
pt_entry* pt1 = (pt_entry*)0x2A000;
/*la tarea Idle tiene que ir en una posicion cualquiera es decir sus tablas de pagina y directorio, siempre que este incluido
en el mapa de memoria de la figura 1.*/
pd_entry* pd_tarea_idle = (pd_entry*) 0x30000;
pt_entry* pt_tarea_idle = (pt_entry*) 0x31000;


void mmu_mapear_pagina(unsigned int virtual, unsigned int cr3, unsigned int fisica, unsigned int attr){
	unsigned int *pde = NULL;
	unsigned int *pt = NULL;
	unsigned int index = 0;

/*pdt: Puntero a la Page Directory Table. Se obtiene con CR3 que tiene el puntero a la primer tabla. Se le agrega el index
que viene en los primeros 10 bits de la direccion virtual.*/
	pde = (unsigned int*)((cr3 & 0xFFFFF000) + (virtual >> 22));
/*indice para la tabla de paginas. Lo & con los 10 bits que voy a usar para la tabla de paginas y nada más. Por ultimo lo
lo shifteo 12 para quedarme con ese. No me interesan los ultimos 12 (los menos significativos) porque solo quiero mapear una
pagina entera.*/
	index = (unsigned int)(virtual & 0x003FF000);
	index = (unsigned int)(index >> 12);
/*Ahora quiero acceder al descriptor dentro de mi PDT que apunta a la page table que necesito entonces tomo ese puntero + 
el indice que calcule antes para llegar hasta el.*/
	pt = (unsigned int*) (*pde & 0xFFFFF000) + index;
/*inserto en esa direccion de la Page Table el descriptor de la page frame que es la direccion fisica mas los attributos 
que le paso por parametro de acceso etc etc etc.*/
	*pt = (fisica | attr);
}
void mmu_unmapear_pagina(unsigned int virtual, unsigned int cr3){
	unsigned int* pdt = NULL;
	unsigned int* pt = NULL;
	unsigned int index = 0;

	pdt = (unsigned int*)((cr3 & 0xFFFFF000) + (virtual >> 22));
	index = (unsigned int)(virtual & 0x003FF000);
	index = (unsigned int)(index >> 12);
	pt = (unsigned int*) (*pdt & 0xFFFFF000) + index;
	*pt = 0; //VERIFICAR: Basta con poner 0 en ese lugar? o sea, tengo que desmapear, no borrar lo que tengo en ese pageframe.	
}

//VERIFICAR: esto es valido? funciona como un puntero realmente lo puedo tratar asi??
unsigned int* free_memory(){
	unsigned int* res = NULL;
	for(unsigned int *i =(unsigned int*) LAST_MEMORY_FREE; i < (unsigned int*)0x9FFFF; i++){
		if(*i != 0){
			res = i;
			if(i == (unsigned int*) 0x9FFFF){
				LAST_MEMORY_FREE = 0x30000;
			}
			LAST_MEMORY_FREE = (unsigned int) i;

			break;
		}
	}
	return res;
}


void mmu_inicializar() {
	//Inicializo la primer entrada del PDT
	pd[0] =(pd_entry){
		.p=0x1,
		.rw=0x1,
		.us=0x0,
		.pwt=0x0,
		.pcd=0x0,
		.a=0x0,
		.ignr=0x0,
		.ps=0x0,
		.g=0x0,
		.ignr_3=0x0,
		.base_12_31 = 0x00028
	};
	pd[1] =(pd_entry){
		.p=0x1,
		.rw=0x1,
		.us=0x0,
		.pwt=0x0,
		.pcd=0x0,
		.a=0x0,
		.ignr=0x0,
		.ps=0x0,
		.g=0x0,
		.ignr_3=0x0,
		.base_12_31 = 0x0002A
	};
	//Pongo el resto de la PDT en 0 ya que no tiene mas entradas.
	for(int i=2; i < 1024; i++){
		pd[i].p = 0;
	}
	for(int k = 0; k < 1024; k++){
		pt[k] = (pt_entry){
			.p=0x1,
			.rw=0x1,
			.us=0x0,
			.pwt=0x0,
			.pcd=0x0,
			.a=0x0,
			.d=0x0,
			.pat=0x0,
			.g=0x0,
			.ignr_3=0x0,
			.base_12_31 = k
		};
	}
	for(int k = 0; k <= 0x77f - 0x400; k++){
		pt1[k] = (pt_entry){
			.p=0x1,
			.rw=0x1,
			.us=0x0,
			.pwt=0x0,
			.pcd=0x0,
			.a=0x0,
			.d=0x0,
			.pat=0x0,
			.g=0x0,
			.ignr_3=0x0,
			.base_12_31 = k + 0x400 //esto es + algo el tema es que no se + que! 
		};
	}
	for(int k = 0x380; k <1024; k++ ){
		pt1[k].p = 0;
	}
}

void mmu_inicializar_dir_tarea(){
	for(int k = 0;k < 1024; k++){
		pd_tarea_idle[k].p = 0;
	}
	pd_tarea_idle[0] =(pd_entry){
		.p=0x1,
		.rw=0x1,
		.us=0x0,
		.pwt=0x0,
		.pcd=0x0,
		.a=0x0,
		.ignr=0x0,
		.ps=0x0,
		.g=0x0,
		.ignr_3=0x0,
		.base_12_31 = 0x00028
	};
	pd_tarea_idle[1] =(pd_entry){
		.p=0x1,
		.rw=0x1,
		.us=0x0,
		.pwt=0x0,
		.pcd=0x0,
		.a=0x0,
		.ignr=0x0,
		.ps=0x0,
		.g=0x0,
		.ignr_3=0x0,
		.base_12_31 = 0x0002A
	};
	///*mi direccion Virtual a mapear a tierra es en realidad el 0x40000000 entonces tomo los primeros 10 bits y me queda el
	//numero 0x100 que en decimal se traduce al numero 64 entonces en mi entrada 64 que vendría a ser mi offset tengo que
	//poner la entrada de directorio que apunte a mi page table. Todo esto esta en la porcion de memoria que dice el mapa de 
	//la figura 1 destinado a paginas de directorio, tablas y pila nivel 0.*/
	pd_tarea_idle[64] = (pd_entry){
		.p=0x1,
		.rw=0x1,
		.us=0x1,
		.pwt=0x0,
		.pcd=0x0,
		.a=0x0,
		.ignr=0x0,
		.ps=0x0,
		.g=0x0,
		.ignr_3=0x0,
		.base_12_31 = 0x31
	};
	for(int i = 0; i < 1024; i++){
		pt_tarea_idle[i].p = 0;
	}
	pt_tarea_idle[0] = (pt_entry){
			.p=0x1,
			.rw=0x1,
			.us=0x1,
			.pwt=0x0,
			.pcd=0x0,
			.a=0x0,
			.d=0x0,
			.pat=0x0,
			.g=0x0,
			.ignr_3=0x0,
			.base_12_31 = 0x20 				//Apunta a esta direccion segun el enunciado. ahi esta alocado el codigo de la tarea Idle.
	};
	pt_tarea_idle[1] = (pt_entry){
			.p=0x1,
			.rw=0x1,
			.us=0x1,
			.pwt=0x0,
			.pcd=0x0,
			.a=0x0,
			.d=0x0,
			.pat=0x0,
			.g=0x0,
			.ignr_3=0x0,
			.base_12_31 = 0x21				//Segunda pagina de la tarea Idle.
	};
	pt_tarea_idle[2] = (pt_entry){
			.p=0x1,
			.rw=0x0,
			.us=0x1,			//VERIFICAR: La Pagina Ancla que va en tierra tiene en el descriptor permiso nivel 3???
			.pwt=0x0,
			.pcd=0x0,
			.a=0x0,
			.d=0x0,
			.pat=0x0,
			.g=0x0,
			.ignr_3=0x0,
			.base_12_31 = 0x00				//Pagina ANCLA.
	};

	//Por ultimo copio todo del SRC al DEST.
	int* IDLE_SRC = (int*) TASK_IDLE_CODE_SRC_ADDR;
	int* IDLE_DEST = (int*) TASK_IDLE_CODE;
	for(int i = 0; i < 1024;i++){
		IDLE_DEST[i] = IDLE_SRC[i];
	}
}


void mmu_inicializar_tareas(){
	for(int i = 0; i < (sizeof(TASK_CODE_SRC_ARRAY)/sizeof(int)); i = i + 0x1000){
		pd_entry* pd = (pd_entry*) free_memory();
		pt_entry* pt = (pt_entry*) free_memory();

		pd[0] =(pd_entry){
			.p=0x1,
			.rw=0x1,
			.us=0x0,
			.pwt=0x0,
			.pcd=0x0,
			.a=0x0,
			.ignr=0x0,
			.ps=0x0,
			.g=0x0,
			.ignr_3=0x0,
			.base_12_31 = (unsigned int)free_memory()
		};
		pd[1] =(pd_entry){
			.p=0x1,
			.rw=0x1,
			.us=0x0,
			.pwt=0x0,
			.pcd=0x0,
			.a=0x0,
			.ignr=0x0,
			.ps=0x0,
			.g=0x0,
			.ignr_3=0x0,
			.base_12_31 = (unsigned int)free_memory()
		};
			for(int j = 0x0; j < 0x400; j++){
				mmu_mapear_pagina(j<<12,(unsigned int)pd_tarea_idle,j<<12,0x03);
			}
			for(int j = 0x400; j <= 0x77F; j++){
				mmu_mapear_pagina(j<<12,(unsigned int)pd_tarea_idle,j<<12,0x03);
			}
			for(int j = 0x780; j < 0x800; j++){
				mmu_mapear_pagina(j<<12,(unsigned int)pd_tarea_idle,j<<12,0x00);
			}
		pd[64] = (pd_entry){		//VERIFICAR: todas las tareas arrancan en 0x100.
			.p=0x1,
			.rw=0x1,
			.us=0x1,
			.pwt=0x0,
			.pcd=0x0,
			.a=0x0,
			.ignr=0x0,
			.ps=0x0,
			.g=0x0,
			.ignr_3=0x0,
			.base_12_31 = ((unsigned int)pt >> 12)
		};
		pt[0] = (pt_entry){
			.p=0x1,
			.rw=0x1,
			.us=0x1,			
			.pwt=0x0,
			.pcd=0x0,
			.a=0x0,
			.d=0x0,
			.pat=0x0,
			.g=0x0,
			.ignr_3=0x0,
			.base_12_31 = (TASK_CODE_SRC_ARRAY[i] >> 12)
		};
		pt[1] = (pt_entry){
			.p=0x1,
			.rw=0x1,
			.us=0x1,			
			.pwt=0x0,
			.pcd=0x0,
			.a=0x0,
			.d=0x0,
			.pat=0x0,
			.g=0x0,
			.ignr_3=0x0,
			.base_12_31 = ((TASK_CODE_SRC_ARRAY[i] + 0x1000) >> 12)
		};
	}
}




