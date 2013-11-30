#ifndef __SCREEEEN_H__
#define __SCREEEEN_H__

#include "defines.h"
extern int get_pagina_fisica(int, int);


//TIPOS
typedef struct str_pixel {unsigned char letra; unsigned char formato;} __attribute__((__packed__)) pixel; //defino pixel
typedef pixel screen[80*25]; //defino pantalla como conjunto de pixeles
typedef char mpus[80*25]; //defino usos de mapa
typedef struct str_estado_de_tarea{
	int eax; int ebx; int ecx; int edx; int esi; 
	int edi; int ebp; int esp; int eip; int cr0; int cr2; int cr3 ;
	int cs; int ds; int es; int fs; int gs; int ss; int eflags;
} __attribute__((__packed__)) estado_task;
//VARIABLES
screen* DISPLAY = (screen *)(VIDEO);
screen* ESTADO = (screen *)(BUFFERESTADO);
screen* MAPA = (screen*)(BUFFERMAPA);
mpus* map_uses = (mpus*) (BUFFERMAPA + (80*25*2));
pixel* ultimo_missil = (pixel*) (0x0);
char pantalla_actual = 1 ;
estado_task estado_error;

//estado_error;

/*estado_error.ebx = 1;
estado_error.ecx = 1;
estado_error.edx = 1;
estado_error.esi = 1;
estado_error.edi = 1;
estado_error.ebp = 1;
estado_error.esp = 1;
estado_error.eip = 1;
estado_error.cr0 = 1;
estado_error.cr2 = 1;
estado_error.cr3 = 1;
estado_error.cs = 1;
estado_error.ds = 1;
estado_error.es = 1;
estado_error.fs = 1;
estado_error.gs = 1;
estado_error.ss = 1;
estado_error.eflags = 1;*/

//
void print_cuadrado(screen *, char, int, int , int, int );
void print_texto(screen *, char *, int, int );
void print_formato(screen *, char, int, int);
void print_pixel(screen *, pixel, int, int);
int pos( int, int);

////Prints en pantalla
//ESTADO
char * nametxt = "Palito Peron Helado/Best Name Ever";
char * naviotxt = "Navio ?";
char * errortxt = "Ningun Error ";
char string_errores[21][10] = {"Error 0", "Error 1","Error 2","Error 3","Error 4","Error 5","Error 6","Error 7","Error 8","Error 9","Error 10",
"Error 11","Error 12","Error 13","Error 14","Error 15","Error 16","Error 17","Error 18","Error 19","Error 20" };

int bandera_x[8] = {1,13,25,37,1,13,25,37};
int bandera_long_x = 10;
int bandera_y[8] = {3,3,3,3,10,10,10,10};
int bandera_long_y = 5;

int tablaerror_x = 49;
int tablaerror_long_x = 29;
int tablaerror_y = 3;
int tablaerror_long_y=11;

int tablatar_x = 2;
int tablatar_long_x = 76;
int tablatar_y = 16;
int tablatar_long_y = 8;

// MAPA

//Cambio pantalla
void load_pantalla();
void cambiar_pantalla(char);
void copiar_buffer(screen *);
void screen_en_negro();

//
void inicializar_pantalla();
void inicializar_pantalla_memoria();

void print_tablaerror();
void print_tablatar(int, int, int, int);
void print_tablatar_from_gdt(int);
void print_tablatar_error(int, int);

void print_texto_cord(screen *, char *, int );
void print_formato_cord(screen *, char , int);
void print_pixel_cord(screen *, pixel, int );
//

void print_clock();



void print_estado_bandera(int, char *);
/*int bandera_posicion_x(int);
int bandera_posicion_y(int);*/
void print_mapa_mem(long, char, char);
void print_error(char);
void print_mapa_from_gdt(int);
void print_pg_mapa(int, int);
void unprint_pg_mapa_from_gdt(int, int);
void print_numero_mapa_cord(int);


void reg_a_string(int , char *, int);
int dir_a_cord(int);
int cord_a_dir(int);

void print_missil_cord(int);
void print_missil(int);

#endif        /* !__SCREEEEN_H__ */