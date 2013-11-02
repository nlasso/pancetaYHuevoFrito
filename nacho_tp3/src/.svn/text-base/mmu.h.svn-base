/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "defines.h"

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


void bleach_pagedir_entry(pagedir_entry *);
void define_pagedir_entry(pagedir_entry *, unsigned char,unsigned char, unsigned long);
void bleach_pagetab_entry(pagetab_entry *);
void define_pagetab_entry(pagetab_entry *, unsigned char,unsigned char, unsigned long);
void mmu_identity_maping(pagedir_entry *, pagetab_entry *, pagetab_entry *) ;

#endif	/* !__MMU_H__ */
