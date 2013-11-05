/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#ifndef __GDT_H__
#define __GDT_H__

#define GDT_COUNT 41

#include "defines.h"


typedef struct str_gdt_descriptor {
    unsigned short  gdt_length;
    unsigned int    gdt_addr;
} __attribute__((__packed__)) gdt_descriptor;

typedef struct str_gdt_entry {
    unsigned short  limit_0_15;
    unsigned short  base_0_15;
    unsigned char   base_16_23;
    unsigned char   type:4;
    unsigned char   s:1;
    unsigned char   dpl:2;
    unsigned char   p:1;
    unsigned char   limit_16_19:4;
    unsigned char   avl:1;
    unsigned char   l:1;
    unsigned char   db:1;
    unsigned char   g:1;
    unsigned char   base_24_31;
} __attribute__((__packed__, aligned (8))) gdt_entry;

void gdt_set_tss(){}

gdt_entry generate_gdt_tss(long unsigned int dir_tss){
    gdt_entry _gdt = (gdt_entry) {   /* Datos Kernel */
    /*  .base_0_15        =   0x0000,   */
    .base_0_15        =   dir_tss & 0xFFFF,
    /*  .base_16_23       =   0x00  , */
    .base_16_23       =   (dir_tss << 16) & 0xFF  ,
    /*  .base_24_31       =   0x00  ,  */
    .base_24_31       =   (dir_tss << 24)  ,
    .limit_0_15       =   0x0068,
    .limit_16_19      =   0x0,             // 0x68
    .type             =   0x9,             //tss no busy             
    .s                =   0x0,             // sistema
    .dpl              =   0x3,             // prioridad
    .avl              =   0x0,             // no se usa
    .p                =   0x1,
    .g                =   0x0,
    .l                =   0x0,             //64 bits
    .db               =   0x0,             //se usan 32 bits?
  };
  return _gdt;
}

/* Tabla GDT */
extern gdt_entry gdt[];
extern gdt_descriptor GDT_DESC;


#endif  /* !__GDT_H__ */
