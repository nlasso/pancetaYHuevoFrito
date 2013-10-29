/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "defines.h"

pd_entry* pd = (pd_entry*) 0x27000;		//Tiene que ser 0x27000 por el enunciado
pt_entry* pt = (pt_entry*) 0x28000;
pt_entry* pt1 = (pt_entry*)0x2A000;
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
		pt1[k] = (pt_entry){
			.p=0x0,
			.rw=0x0,
			.us=0x0,
			.pwt=0x0,
			.pcd=0x0,
			.a=0x0,
			.d=0x0,
			.pat=0x0,
			.g=0x0,
			.ignr_3=0x0,
			.base_12_31 = 0x0	//esto es + algo el tema es que no se + que! 
		};
	}
}

