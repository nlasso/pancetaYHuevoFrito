/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "defines.h"
#include "idt.h"
#include "isr.h"

idt_entry idt[256] = { };

idt_descriptor IDT_DESC = {
    sizeof(idt) - 1,
    (unsigned int) &idt
};


/*
    La siguiente es una macro de EJEMPLO para ayudar a armar entradas de
    interrupciones. Para usar, descomentar y completar CORRECTAMENTE los
    atributos y el registro de segmento. Invocarla desde idt_inicializar() de
    la siguiene manera:

    void idt_inicializar() {
        IDT_ENTRY(0);
        ...
        IDT_ENTRY(19);

        ...
    }
*/


#define IDT_ENTRY(numero)   \
    idt[numero].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## numero) & (unsigned int) 0xFFFF);\
    idt[numero].segsel = (unsigned short) (GDT_IDX_CODE_0 * 8);\
    idt[numero].attr = (unsigned short) 0x8E00;\
    idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 & (unsigned int) 0xFFFF);\


void idt_inicializar() {
    IDT_ENTRY(0);
    IDT_ENTRY(1);
    IDT_ENTRY(2);
    IDT_ENTRY(3);
    IDT_ENTRY(4);
    IDT_ENTRY(5);
    IDT_ENTRY(6);
    IDT_ENTRY(7);
    IDT_ENTRY(8);
    IDT_ENTRY(9);
    IDT_ENTRY(10);
    IDT_ENTRY(11);
    IDT_ENTRY(12);
    IDT_ENTRY(13);
    IDT_ENTRY(14);
    IDT_ENTRY(15);
    IDT_ENTRY(16);
    IDT_ENTRY(17);
    IDT_ENTRY(18);
    IDT_ENTRY(19);
    /*IDT_ENTRY(INTBANDERA);
    IDT_ENTRY(INTCLOCK);
    IDT_ENTRY(INTKEYBOARD);
    IDT_ENTRY(INTSERVICIOS);*/

    //Inicializo todas las entradas de la IDT de la 32 a la 255
    int i = 20;
    while(i < 256){
        idt[i].offset_0_15 = (unsigned short) ((unsigned int)(&int_invalida) & (unsigned int) 0xFFFF);
        idt[i].segsel = (unsigned short) (GDT_IDX_CODE_0 * 8);   //VERIFICAR: No estoy seguro de esto.
        idt[i].attr = (unsigned short) 0x8E00;
        idt[i].offset_16_31 = (unsigned short) ((unsigned int)(&int_invalida) >> 16 & (unsigned int) 0xFFFF);
        i++;
    }

    //Interrupcion de reloj.
    idt[INTCLOCK].offset_0_15 = (unsigned short) ((unsigned int)(&screen_proximo_reloj) & (unsigned int) 0xFFFF);
    idt[INTCLOCK].segsel = (unsigned short) (GDT_IDX_CODE_0 * 8);    //VERIFICAR: No estoy seguro de esto.
    idt[INTCLOCK].attr = (unsigned short) 0x8E00;
    idt[INTCLOCK].offset_16_31 = (unsigned short) ((unsigned int)(&screen_proximo_reloj) >> 16 & (unsigned int) 0xFFFF);

    //Interrupcion de Teclado.

    idt[INTKEYBOARD].offset_0_15 = (unsigned short) ((unsigned int)(&int_teclado) & (unsigned int) 0xFFFF);
    idt[INTKEYBOARD].segsel = (unsigned short) (GDT_IDX_CODE_0 * 8);     //VERIFICAR: No estoy seguro de esto.
    idt[INTKEYBOARD].attr = (unsigned short) 0x8E00;
    idt[INTKEYBOARD].offset_16_31 = (unsigned short) ((unsigned int)(&int_teclado) >> 16 & (unsigned int) 0xFFFF);

    //interrrupcion de software para servicios

    idt[INTSERVICIOS].offset_0_15 = (unsigned short) ((unsigned int)(&int_servicios) & (unsigned int) 0xFFFF);
    idt[INTSERVICIOS].segsel = (unsigned short) (GDT_IDX_CODE_0 * 8);    // HAY QUE CAMBIAR ESTO
    idt[INTSERVICIOS].attr = (unsigned short) 0x8E00; // HAY QUE CAMBIAR ESTO
    idt[INTSERVICIOS].offset_16_31 = (unsigned short) ((unsigned int)(&int_servicios) >> 16 & (unsigned int) 0xFFFF);

    //Interrupcion de bandera.

    idt[INTBANDERA].offset_0_15 = (unsigned short) ((unsigned int)(&int_bandera) & (unsigned int) 0xFFFF);
    idt[INTBANDERA].segsel = (unsigned short) (GDT_IDX_CODE_0 * 8);      // HAY QUE CAMBIAR ESTO
    idt[INTBANDERA].attr = (unsigned short) 0x8E00; // HAY QUE CAMBIAR ESTO
    idt[INTBANDERA].offset_16_31 = (unsigned short) ((unsigned int)(&int_bandera) >> 16 & (unsigned int) 0xFFFF);
}





    /*0   Divide-by-zero  fault
    1   Debug exception trap or fault
    2   Non-Maskable Interrupt (NMI)    trap
    3   Breakpoint (INT 3)  trap
    4   Overflow (INTO with EFlags[OF] set) trap
    5   Bound exception (BOUND on out-of-bounds access) trap
    6   Invalid Opcode  trap
    7   FPU not available   trap
    8*  Double Fault    abort
    9   Coprocessor Segment Overrun abort
    10* Invalid TSS fault
    11* Segment not present fault
    12* Stack exception fault
    13* General Protection  fault or trap
    14* Page fault  fault
    15  Reserved
    16  Floating-point error    fault
    17  Alignment Check fault
    18  Machine Check   abort
    19 SIMD */
    // Excepciones

