#ifndef __SCREEEEN_H__
#define __SCREEEEN_H__

#include "defines.h"


//
char pantalla_actual = 1 ;

void inicializar_pantalla();

void print_clock();
void load_pantalla();

void cambiar_pantalla(char);
void print_pixel(char *, int, int, char, char);
void print_texto(char *, int, int, char *, char);
void print_estado_bandera(int, char *);
int bandera_posicion_x(int);
int bandera_posicion_y(int);
void print_mapa_mem(long, char, char);
void print_error(char);

#endif	/* !__SCREEEEN_H__ */