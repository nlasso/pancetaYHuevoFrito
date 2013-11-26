#ifndef __SCREEEEN_H__
#define __SCREEEEN_H__

#include "defines.h"


//TIPOS
typedef struct str_pixel {unsigned char  letra; unsigned char  formato;} __attribute__((__packed__)) pixel; //defino pixel
typedef pixel screen[80*25]; //defino pantalla como conjunto de pixeles
typedef char mpus[80*25]; //defino usos de mapa


//VARIABLES
screen* DISPLAY = (screen *)(VIDEO); 
screen* ESTADO = (screen *)(BUFFERESTADO);
screen* MAPA = (screen*)(BUFFERMAPA);
mpus*  map_uses = (mpus*) (BUFFERMAPA + (80*25*2));
char pantalla_actual = 1 ;

//
void print_cuadrado(screen *, char, int, int , int, int );
void print_pixel(screen *, pixel, int, int);
int pos( int, int);

////Configuracion de pantalla
//ESTADO 
int bandera_x[8] = {1,13,25,37,1,13,25,37};
int bandera_long_x = 10;
int bandera_y[8] = {2,2,2,2,9,9,9,9};
int bandera_long_y =  5;
// MAPA

void mpus_quitar(int, int, int);
void mpus_agregar(int, int, int);
char mpus_obtener(int, int);

//Cambio pantalla
void load_pantalla();
void cambiar_pantalla(char);
void copiar_buffer(screen *);
void screen_en_negro();

//
void inicializar_pantalla();





//

void print_clock();


//void print_pixel(char *, int, int, char, char);
void print_texto(char *, int, int, char *, char);
void print_estado_bandera(int, char *);
/*int bandera_posicion_x(int);
int bandera_posicion_y(int);*/
void print_mapa_mem(long, char, char);
void print_error(char);

#endif	/* !__SCREEEEN_H__ */