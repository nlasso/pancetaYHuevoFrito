#include "screen.h"
#include "colors.h"
#include "defines.h"

void screen_en_negro(){
	pixel pixel_negro;
	pixel_negro.letra = *((char *) " ");
	pixel_negro.formato = (C_FG_BLACK | C_BG_BLACK);
	int x = 0;
	x= 0; while(x< 2000){(*DISPLAY)[x] = pixel_negro; x++;}
}

void inicializar_pantalla(){
	int x;

	//Limpio mpus
	x= 0; while(x< 2000){
		(*map_uses)[x] = 0; x++;
	}

	pixel pixel_feo;
	pixel_feo.letra = *((char *) "%");
	pixel_feo.formato = (C_FG_BLACK | C_BG_CYAN | C_BLINK);
	screen* buffer = ESTADO;	x = 0;
	while(x<(2000)){(*buffer)[x] = pixel_feo; x++;}
	
	x = 0; int x1, x2, y1, y2;
	while(x < CANT_TAREAS ){
		x1 = bandera_x[x]; x2 = bandera_long_x + x1 -1;
		y1 = bandera_y[x]; y2 = bandera_long_y + y1 -1;
		print_cuadrado(buffer, (C_FG_BLACK | C_BG_BROWN ), x1, y1, x2, y2);	
		x++;
	}
	//print_cuadrado(buffer, (C_FG_BLACK | C_BG_BROWN ), 1, 1, 1, 5 );


	cambiar_pantalla(SCREENESTADO);
	load_pantalla();
	/*cambiar_pantalla(SCREENMAPA);
	load_pantalla();*/
}

//Copiar pantalla
void cambiar_pantalla(char pant){pantalla_actual = pant;};

void load_pantalla(){
	screen* buffer = ESTADO ;	
	if(pantalla_actual == SCREENMAPA){ buffer = MAPA;};
	copiar_buffer(buffer);
}

void copiar_buffer(screen * buffer){
	screen* pantalla = DISPLAY;	int x = 0;
	while(x<(1920)){(*pantalla)[x] = (*buffer)[x];x++;}
}

void print_cuadrado(screen * pantalla, char format, int x1, int y1, int x2, int y2){
	pixel pixel_cuadrado;	
	pixel_cuadrado.letra = ASCII_space;
	pixel_cuadrado.formato = format;
	int _y = 0; 
	while(_y <= (y2 - y1)){
		int _x = 0; 
		while( _x <= (x2 - x1)){
			print_pixel(pantalla, pixel_cuadrado, _x+x1, _y+y1);_x++;
		}_y++;
	};
};

void print_pixel(screen * pantalla, pixel pix, int x, int y){	(*pantalla)[pos(x,y)] = pix;}

int pos(int x, int y){return (x+ (y*80));};

void print_error(char error){};

/*void print_error(char error){
	//error -= ASCII_first_num;
	char * posicion = (char *) BUFFERESTADO;
	char * stringz = "Error fatal xx";
	char decimal = 0;
	while(error > 9){
		decimal ++;
		error -= 10;
	}
	stringz[12] = ASCII_first_num + decimal;
	stringz[13] = ASCII_first_num + error;
	//stringz[13] = ASCII_first_num + error;
	print_texto(posicion,TABLAERRORES_X, TABLAERRORES_Y, stringz, (C_FG_BLACK | C_BG_CYAN) );
	load_pantalla();
}


void print_texto(char * _pantalla, int columna, int fila, char * texto, char formato){
	int y = 0;
	while(texto[y] != 0){print_pixel(_pantalla, columna + y, fila, texto[y], formato); y++;}
}*/

/*void print_pixel(char * _pantalla, int columna, int fila, char caracter, char formato){
	int pixel_actual = fila * 80;
	pixel_actual += columna;
	pixel_actual = pixel_actual * 2;
	_pantalla[pixel_actual] = caracter;
	_pantalla[pixel_actual+1] = formato;
}*/