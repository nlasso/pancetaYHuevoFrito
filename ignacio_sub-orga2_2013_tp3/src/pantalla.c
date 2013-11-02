#include "pantalla.h"
#include "colors.h"
#include "defines.h"

/*char s_void [80*25+1];
int sv_zero = 80*25;
s_void[80*25] = 0;
void start_svoid(){
	int sv_count = 0;
	while(sv_count < sv_zero){s_void[sv_count] = ASCII_space;sv_count++;};
};*/


void inicializar_pantalla(){
	int x; int y; char * posicion; char * stringz; char * stringw;
	char letra; char formato;
	posicion = (char *) BUFFERESTADO;
	//DIBUJO MAPA BUFFER ESTADO
	x = 0; y = 0;
	//PRINT TODA LA PAGINA DE BLANCO
	letra =	0X20;
	formato = C_BG_LIGHT_GREY;
	while(y<24){
		x = 0;
		while(x<80){print_pixel(posicion, x, y, letra, formato); x++;}
		y++;
	}
	//PRINT FRANJA SUPERIOR DE NEGRO
	x = 0;
	letra = 0x20;
	formato = (C_FG_WHITE | C_BG_BLACK );
	while(x < 80){ print_pixel(posicion, x, 0, letra, formato); x++;}

	//PRINT NOMBRE DE GRUPO
	stringz = NAMEGRUPO;
	y = 0; x = 0;
	print_texto(posicion, x, y, stringz, formato);
	//PRINT TABLA ERRORES
	stringz = "                             ";
	x = TABLAERRORES_X;
	y = TABLAERRORES_Y;
	char color;
	color = (C_FG_BLACK | C_BG_CYAN);
	
	while(y < TABLAERRORES_ALTO + 1){
		print_texto(posicion, x, y, stringz, color);
		color = (C_FG_WHITE | C_BG_BLACK);
		y++;		
	}


	x = 1;
	stringz = "NAVIO Z";
	stringw = "          ";
	int stringflagnum = 6;//STRINGFLAGNUM ES EL ULTIMO CARACTER DEL STRING
	while (x < 9){ //X es el FLAG actual
		stringz[stringflagnum] = ASCII_first_num + x;
		int pos_x = bandera_posicion_x(x);
		int pos_y = bandera_posicion_y(x);
		y = 0; //Y es el caracter actual del string
		print_texto(posicion,pos_x + 3 , pos_y - 1, stringz,  (C_FG_BLACK | C_BG_LIGHT_GREY ));
		y = 0; //columna
		while(y < 5){print_texto(posicion, pos_x, pos_y+y, stringw, C_BG_BROWN); y++;}
		x++;
	}

	//PRINT TABLA INFERIOR
	stringz = "                                                                             ";
	x = 2;
	y = 16;
	color = (C_FG_BLACK | C_BG_CYAN);
	while(y < 24){
		print_texto(posicion, x, y, stringz, color); 	y++;
	}
	y = 16;
	x = 1;
	formato = ((C_FG_CYAN | C_BG_RED));
	while(y < 24){
		print_pixel(posicion, x, y, ASCII_first_num + y - 15, formato); 	y++;
	}


	posicion = (char *) BUFFERMAPA;

	x = 0;
	y = 256*2; // Un 1 mb divido 4kb
	while(x< y){
		posicion[x] = 57;						x++; 
		posicion[x] = (C_FG_BLUE | C_BG_GREEN);	x++;
	}
	y += ((256*2) *6); 	// Un  6 mb divido 4kb
	y += 256; 			// Un 6.5mb divido 4kb
	//y = (1920*2);
	while(x < y){
		posicion[x] = 38;						x++; 
		posicion[x] = (C_FG_RED | C_BG_BLUE);	x++;
	}

	posicion = (char *) VIDEO;
	x = 0;
	while(x<(2000*2)){
		posicion[x] = 33; 			x++;
		posicion[x] = C_BG_RED ; 	x++;
	}

}

void load_pantalla(){
	char * posicion;
	posicion = (char *) BUFFERMAPA;
	if(pantalla_actual == 1){ posicion = (char *) BUFFERESTADO;};
	char * video = (char *) VIDEO; 
	int x = 0;
	while(x<(1920*2)){
		video[x] = posicion[x];
		x++;
		video[x] = posicion[x];
		x++; 
	}
}
void cambiar_pantalla(char pant){
	pantalla_actual = pant;
}
//NOTA: BANDERAS VAN DE 1 AL 8, SIN 0
int bandera_posicion_x(int flagsy){
	int pos = 2;
	if(flagsy>4){flagsy-=4;}
	while(flagsy!=1){ pos+=12; flagsy--;}
	return pos;
};

int bandera_posicion_y(int flagsy){
	int pos = 3;
	if(flagsy > 4){	pos = 10;}
	return pos;
};


/*void print_estado_pixel(int columna, int fila, char caracter, char formato){
	char * posicion = (char *) BUFFERESTADO; print_pixel(posicion, columna, fila, caracter, formato);
}bochs

void print_mapa_pixel(int columna, int fila, char caracter, char formato){
	char * posicion = (char *) BUFFERMAPA; print_pixel(posicion, columna, fila, caracter, formato);
}*/

void print_error(char error){
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
}

void print_pixel(char * _pantalla, int columna, int fila, char caracter, char formato){
	int pixel_actual = fila * 80;
	pixel_actual += columna;
	pixel_actual = pixel_actual * 2;
	_pantalla[pixel_actual] = caracter;
	_pantalla[pixel_actual+1] = formato;
}