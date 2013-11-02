/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "defines.h"
#include "idt.h"
#include "isr.h"

idt_entry idt[255] = { };

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
    idt[numero].segsel = (unsigned short) 0x0090;\
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
    IDT_ENTRY(31);
    IDT_ENTRY(32);
    IDT_ENTRY(50);
    IDT_ENTRY(66);
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

