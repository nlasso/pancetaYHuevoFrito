/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "colors.h"
#include "defines.h"
#include "game.h"
#include "syscall.h"

#define BANDERA_BUFFER  0x40001000

/*
 * Estructura de la tarea :
 *
 *  0x40000000 - 0x40001000 : codigo tarea          (   5 KB)
 *  0x40001000 - 0x40001400 : area de banderas      (   1 KB)
 *  0x40001400 - 0x40001c00 : pila tarea            (   2 KB)
 *  0x40001c00 - 0x40001ffc : pila bandera          (1020 B )
 *  0x40001ffc - 0x40002000 : dir. funcion bandera  (   4 B )
 */

/* Estructura de para acceder a memoria de video */
typedef struct {
    unsigned char c;
    unsigned char a;
} ca;

unsigned char * bandera();

// global var
#define GLOBAL_START 0x40000000  
#define var_B GLOBAL_START+0x2000-0x200

void task() {
    int j;
    while(1){ j = 27;
        j = 10;
        j = 30;
        j = j;}    
    /* Tarea 1 */
    unsigned int i=0;
    while(1) {
      for(i=0;i<0x000FF000;i=i+0x1000){
        syscall_fondear(i);
      }
    };
}

unsigned char * bandera() {
    int a = 90;
    a = 91;
    a = 92;
    a = a;
    return 0;
}