/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "defines.h"
#include "schedule_structure.h"
#include "i386.h"

typedef struct str_pagedir_entry {
    unsigned char   present:1;
    unsigned char   writable:1;
    unsigned char   priviledge:1;
    unsigned char   write_through:1;
    unsigned char   cache_disable:1;
    unsigned char   accessed:1;
    unsigned char   ignored:1;
    unsigned char   page_size:1;
    unsigned char   global:1;
    unsigned short  disponible_9_11:3;
    unsigned int    dirbase_12_31:20;
} __attribute__((__packed__, aligned (4))) pagedir_entry;

typedef struct str_pagetab_entry {
    unsigned char   present:1;
    unsigned char   writable:1;
    unsigned char   priviledge:1;
    unsigned char   write_through:1;
    unsigned char   cache_disable:1;
    unsigned char   accessed:1;
    unsigned char   dirty:1;
    unsigned char   page_attribute_index:1;
    unsigned char   global:1;
    unsigned short  disponible_9_11:3;
    unsigned int    dirbase_12_31:20;
} __attribute__((__packed__, aligned (4))) pagetab_entry;


// CREACION DIRS
void bleach_pagedir_entry(pagedir_entry *);
void define_pagedir_entry(pagedir_entry *, unsigned char,unsigned char, unsigned long);
void bleach_pagetab_entry(pagetab_entry *);
void define_pagetab_entry(pagetab_entry *, unsigned char,unsigned char, unsigned long);
// 
unsigned int get_pagedir_entry_fisica(long unsigned int, long unsigned int);
pagetab_entry * get_descriptor(unsigned int, unsigned int);

//
int get_pagina_fisica(int, int);
int get_pagina_fisica_tarea(int, int);

// INICIALIZACION
void mmu_inicializar();
void mmu_identity_maping();
void mmu_inicializar_tareas();
void mmu_tareas_a_mar();
// SYSCALLS
void canionear(unsigned int, unsigned int*);
void navegar(unsigned int, unsigned int);
void anclar(unsigned int);

void reubicar_pagina(unsigned int, unsigned int, unsigned int);
void clonar_pagina(unsigned int, unsigned int);

void mmu_mapear_pagina(unsigned int, unsigned int, unsigned int, unsigned char);
void mmu_unmapear_pagina(unsigned int, unsigned int);
void mmu_backdoor_mapping_task(unsigned int, int);
void mmu_backdoor_unmapping(unsigned int, int);
void flush_tables();

//NECESITO REPETIR LA ESTRUCTURA DEL SCHEDULE_T PARA QUE ME PERMITA TRAER LA VARIALBE DE OTRO LADO

#endif	/* !__MMU_H__ */

