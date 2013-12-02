/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#include "gdt.h"
#include "tss.h"
extern tss tarea_inicial;
extern tss tarea_idle;
extern tss tss_navios[CANT_TAREAS];
extern tss tss_banderas[CANT_TAREAS];

// NOTA 1: AVAILABLE FOR SOFTWARE NO SABEMOS QUE HACE, LO PONEMOS EN 0
// NOTA 2: TIPO DATOS ES 2 Y CODIGO ES A. DESPUES REVISAR.
gdt_entry gdt[GDT_COUNT] = {
  /* Descriptor nulo */
  /* Offset = 0x00 */
  [GDT_IDX_NULL_DESC] = (gdt_entry) {
    (unsigned short)    0x0000,               /* limit[0:15]  */
    (unsigned short)    0x0000,               /* base[0:15]   */
    (unsigned char)     0x00,                 /* base[23:16]  */
    (unsigned char)     0x00,                 /* type         */
    (unsigned char)     0x00,                 /* s            */
    (unsigned char)     0x00,                 /* dpl          */
    (unsigned char)     0x01,                 /* p            */
    (unsigned char)     0x00,                 /* limit[16:19] */
    (unsigned char)     0x00,                 /* avl          */
    (unsigned char)     0x00,                 /* l            */
    (unsigned char)     0x00,                 /* db           */
    (unsigned char)     0x00,                 /* g            */
    (unsigned char)     0x00,                 /* base[31:24]  */
  },
  [GDT_IDX_DATA_0] = (gdt_entry) {   /* Datos Kernel */
    .base_0_15        =   0x0000,
    .base_16_23       =   0x00  ,
    .base_24_31       =   0x00  ,
    .limit_0_15       =   0xFFFF,
    .limit_16_19      =   0x6   ,             // 0x6FFFF000
    .type             =   0x2   ,             
    .s                =   0x1,                // sistema/usuario
    .dpl              =   0x0,                // prioridad
    .avl              =   0x0,                // no se usa
    .p                =   0x1,
    .g                =   0x1,
    .l                =   0x0,                //64 bits
    .db               =   0x1,                //se usan 32 bits?
  },
  [GDT_IDX_CODE_0] = (gdt_entry){                                           /* Código Kernel */
    .base_0_15        =   0x0000,
    .base_16_23       =   0x00  ,
    .base_24_31       =   0x00  ,
    .limit_0_15       =   0xFFFF,
    .limit_16_19      =   0x6   ,             // 0x6FFFF000
    //ACA EL TP BACON TENIA 0X08
    .type             =   0x8   ,             
    .s                =   0x1,                // sistema/usuario
    .dpl              =   0x0,                // prioridad
    .avl              =   0x0,                // no se usa
    .p                =   0x1,
    .g                =   0x1,
    .l                =   0x0,                //64 bits
    .db               =   0x1,                //se usan 32 bits?
  },
  [GDT_IDX_DATA_3] = (gdt_entry) {                     /* Datos Usuario */
    .base_0_15        =   0x0000,
    .base_16_23       =   0x00  ,
    .base_24_31       =   0x00  ,
    .limit_0_15       =   0xFFFF,
    .limit_16_19      =   0x6   ,             // 0x6FFFF000
    .type             =   0x2   ,
    .s                =   0x1,                // sistema/usuario
    .dpl              =   0x3,                // prioridad
    .avl              =   0x0,                // no se usa
    .p                =   0x1,
    .g                =   0x1,
    .l                =   0x0,                //64 bits
    .db               =   0x1,                //se usan 32 bits?
  },
   [GDT_IDX_CODE_3] = (gdt_entry) {                  /* Código Usuario */
    .base_0_15        =   0x0000,
    .base_16_23       =   0x00  ,
    .base_24_31       =   0x00  ,
    .limit_0_15       =   0xFFFF,
    .limit_16_19      =   0x6   ,             // 0x6FFFF000
    //ACA EL TP BACON TENIA 0X8
    .type             =   0xA   ,
    .s                =   0x1,                // sistema/usuario
    .dpl              =   0x3,                // prioridad
    .avl              =   0x0,                // no se usa
    .p                =   0x1,
    .g                =   0x1,
    .l                =   0x0,                //64 bits
    .db               =   0x1,                //se usan 32 bits?
  },
   [GDT_IDX_VIDEO] = (gdt_entry) {   /* Video */
    .base_0_15        =   0x8000,
    .base_16_23       =   0x0B  ,
    .base_24_31       =   0x00  ,
    .limit_0_15       =   0x0F9F,             // 0xFA0 (4000) -1
    .limit_16_19      =   0x0   ,             // 0x000B8888F9F
    .type             =   0x2   ,
    .s                =   0x1,                // sistema/usuario
    .dpl              =   0x0,                // prioridad
    .avl              =   0x0,                // no se usa
    .p                =   0x1,
    .g                =   0x0,
    .l                =   0x0,                //64 bits
    .db               =   0x1,                //se usan 32 bits?
  }, 
  //   
  // DEFINO TODAS LAS ENTRADAS DE TSS EN BLANCO
  // ESTO ES POR QUE NO PUEDO DEFINIRLAS DINAMICAMENTE
 /* [GDT_TSS_INICIAL] = (gdt_entry) {(unsigned short)0x0000,(unsigned short)0x0000,(unsigned char)0x00,
    (unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,
    (unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,},
  [GDT_TSS_IDLE]    = (gdt_entry) {(unsigned short)0x0000,(unsigned short)0
  x0000,(unsigned char)0x00,
    (unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,
    (unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,}, 
  [GDT_TSS_TS1]     = (gdt_entry) {(unsigned short)0x0000,(unsigned short)0x0000,(unsigned char)0x00,
    (unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,
    (unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,},
  [GDT_TSS_FG1]     = (gdt_entry) {(unsigned short)0x0000,(unsigned short)0x0000,(unsigned char)0x00,
    (unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,
    (unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,},*/
};

void gdt_set_tss(){//esto se tiene que correr una vez creadas las tss de las tareas, sino da error
    gdt[GDT_TSS_INICIAL] = generate_gdt_tss((long unsigned int)&tarea_inicial); gdt[GDT_TSS_INICIAL].dpl = 0x0;
    gdt[GDT_TSS_IDLE] = generate_gdt_tss((long unsigned int)&tarea_idle);       gdt[GDT_TSS_IDLE].dpl    = 0x0;
    gdt[GDT_TSS_TS1] = generate_gdt_tss((long unsigned int)&tss_navios[0]);
    gdt[GDT_TSS_FG1] = generate_gdt_tss((long unsigned int)&tss_banderas[0]);
    gdt[GDT_TSS_TS2] = generate_gdt_tss((long unsigned int)&tss_navios[1]);
    gdt[GDT_TSS_FG2] = generate_gdt_tss((long unsigned int)&tss_banderas[1]);
    gdt[GDT_TSS_TS3] = generate_gdt_tss((long unsigned int)&tss_navios[2]);
    gdt[GDT_TSS_FG3] = generate_gdt_tss((long unsigned int)&tss_banderas[2]);
    gdt[GDT_TSS_TS4] = generate_gdt_tss((long unsigned int)&tss_navios[3]);
    gdt[GDT_TSS_FG4] = generate_gdt_tss((long unsigned int)&tss_banderas[3]);
    gdt[GDT_TSS_TS5] = generate_gdt_tss((long unsigned int)&tss_navios[4]);
    gdt[GDT_TSS_FG5] = generate_gdt_tss((long unsigned int)&tss_banderas[4]);
    gdt[GDT_TSS_TS6] = generate_gdt_tss((long unsigned int)&tss_navios[5]);
    gdt[GDT_TSS_FG6] = generate_gdt_tss((long unsigned int)&tss_banderas[5]);
    gdt[GDT_TSS_TS7] = generate_gdt_tss((long unsigned int)&tss_navios[6]);
    gdt[GDT_TSS_FG7] = generate_gdt_tss((long unsigned int)&tss_banderas[6]);
    gdt[GDT_TSS_TS8] = generate_gdt_tss((long unsigned int)&tss_navios[7]);
    gdt[GDT_TSS_FG8] = generate_gdt_tss((long unsigned int)&tss_banderas[7]);

};

gdt_entry generate_gdt_tss(long unsigned int dir_tss){
    gdt_entry _gdt = (gdt_entry) {   /* Datos Kernel */
    /*  .base_0_15        =   0x0000,   */
    .base_0_15        =   dir_tss & 0xFFFF,
    /*  .base_16_23       =   0x00  , */
    .base_16_23       =   (dir_tss << 16) & 0xFF  ,
    /*  .base_24_31       =   0x00  ,  */
    .base_24_31       =   (dir_tss << 24)  ,
    .limit_0_15       =   0x0067,
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


gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (unsigned int) &gdt
};
