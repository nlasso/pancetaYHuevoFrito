/* ** por compatibilidad se omiten tildes **

================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#include "gdt.h"

gdt_entry gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] = (gdt_entry) {
        (unsigned short)    0x0000,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x00,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x00,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    [GDT_IDX_CODE_0] = (gdt_entry) {
        .limit_0_15 = 0xffff,
        .base_0_15 = 0x0000,
        .base_23_16 = 0x00,
        .type = 0x08,           
        .s = 0x01,           
        .dpl = 0x00,           
        .p = 0x01,           
        .limit_16_19 = 0x06,           
        .avl = 0x00,        
        .l = 0x00,        
        .db = 0x01,        
        .g = 0x01,           
        .base_31_24 = 0x00,           
    },
        [GDT_IDX_CODE_3] = (gdt_entry) {
        .limit_0_15 = 0xffff,
        .base_0_15 = 0x0000,
        .base_23_16 = 0x00,
        .type = 0x08,           
        .s = 0x01,           
        .dpl = 0x03,           
        .p = 0x01,           
        .limit_16_19 = 0x06,           
        .avl = 0x00,        
        .l = 0x00,        
        .db = 0x01,        
        .g = 0x01,           
        .base_31_24 = 0x00,
    },
        [GDT_IDX_DATA_0] = (gdt_entry) {
        .limit_0_15 = 0xffff,
        .base_0_15 = 0x0000,
        .base_23_16 = 0x00,
        .type = 0x02,           
        .s = 0x01,           
        .dpl = 0x00,           
        .p = 0x01,           
        .limit_16_19 = 0x06,           
        .avl = 0x00,        
        .l = 0x00,        
        .db = 0x01,        
        .g = 0x01,           
        .base_31_24 = 0x00,
    },
        [GDT_IDX_DATA_3] = (gdt_entry) {
        .limit_0_15 = 0xffff,
        .base_0_15 = 0x0000,
        .base_23_16 = 0x00,
        .type = 0x02,           
        .s = 0x01,           
        .dpl = 0x03,           
        .p = 0x01,           
        .limit_16_19 = 0x06,           
        .avl = 0x00,        
        .l = 0x00,        
        .db = 0x01,        
        .g = 0x01,           
        .base_31_24 = 0x00,
    },
        [GDT_IDX_VIDEO] = (gdt_entry) {
        .limit_0_15 = 0xffff,
        .base_0_15 = 0x8000,
        .base_23_16 = 0x0B,
        .type = 0x02,           
        .s = 0x01,           
        .dpl = 0x03,           
        .p = 0x00,           
        .limit_16_19 = 0x07,           
        .avl = 0x00,        
        .l = 0x00,        
        .db = 0x01,        
        .g = 0x00,           
        .base_31_24 = 0x00,
    },
};

gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (unsigned int) &gdt
};
