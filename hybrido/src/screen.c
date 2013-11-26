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
	int x, x1, x2, y1, y2;
	pixel pixel_actual;
	char * c;

	//Limpio mpus
	x= 0; while(x< 2000){
		(*map_uses)[x] = 0; x++;
	}

	//////
	/// GENERO BUFFER ESTADO
	/////
	screen* buffer = ESTADO;

	//pongo todo de blanco
	pixel_actual.letra = *((char *) " ");
	pixel_actual.formato = (C_FG_BLACK | C_BG_LIGHT_GREY);
	x = 0;
	while(x<(1920)){(*buffer)[x] = pixel_actual; x++;}
	
	// creo franja negra
	x1 = 0; x2 = 79; y1 = 0; y2 = 0;
	print_cuadrado(buffer, (C_FG_WHITE | C_BG_BLACK),x1,y1,x2,y2);
	print_texto(buffer, nametxt, 0, 0);

	//creo banderas
	x = 0;
	while(x < CANT_TAREAS ){
		x1 = bandera_x[x]; x2 = bandera_long_x + x1 -1;
		y1 = bandera_y[x]; y2 = bandera_long_y + y1 -1;
		print_cuadrado(buffer, (C_FG_BLACK | C_BG_RED ), x1, y1, x2, y2);	

		y1 -= 1; x1 += 3; naviotxt[6] = ASCII_first_num + x + 1;
		print_texto(buffer, naviotxt, x1, y1);
		x++;
	}
	
	//creo franja azul de tabla de errores
	x1 = tablaerror_x; x2 = tablaerror_long_x + x1;
	y1 = tablaerror_y - 1; y2 = y1;
	print_cuadrado(buffer, (C_FG_BLACK | C_BG_CYAN),x1,y1,x2,y2);
	print_texto(buffer, errortxt,x1, y1 );

	//creo tabla de errores
	x1 = tablaerror_x; x2 = tablaerror_long_x + x1;
	y1 = tablaerror_y; y2 = tablaerror_long_y + y1;
	print_cuadrado(buffer, (C_FG_WHITE | C_BG_BLACK),x1,y1,x2,y2);

	//creo franja roja de tabla de tareas
	x1 = tablatar_x - 1; x2 = x1;
	y1 = tablatar_y; y2 = tablatar_long_y + y1;
	print_cuadrado(buffer, (C_FG_BLACK | C_BG_RED), x1, y1, x2, y2);
	x2 = y1; c = (char *) "1";
	while((int)(* c ) < (int)(*(char *) "9")){print_texto(buffer, c, x1, x2); x2++;(*c)++;}

	// creo el resto de la tablas de tarea
	x1 = tablatar_x; x2 = tablatar_long_x + x1;
	print_cuadrado(buffer, (C_FG_WHITE | C_BG_CYAN), x1, y1, x2, y2);

	// Imprime ejemplo de errores
	int b = 0xA1020B01;
	print_tablaerror(b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b);

	// Imprime ejemplo tareas
	print_tablatar_from_gdt(1);
	print_tablatar_from_gdt(2);
	print_tablatar_from_gdt(3);
	print_tablatar_from_gdt(4);
	print_tablatar_from_gdt(5);
	print_tablatar_from_gdt(6);
	print_tablatar_from_gdt(7);
	print_tablatar_from_gdt(8);

	print_tablatar_error(3,5);


	cambiar_pantalla(SCREENESTADO);
	load_pantalla();
	//////
	/// GENERO BUFFER MAPA
	/////
	buffer = MAPA;
	x1 = 0; x2 = 79; y1= 0; y2 = 2;
	print_cuadrado(buffer, (C_FG_BLACK | C_BG_GREEN), x1, y1, x2, y2);
	x1 = 0; x2 = 79; y1= 3; y2 = 24;
	print_cuadrado(buffer, (C_FG_BLACK | C_BG_BLUE), x1, y1, x2, y2);
	x1 = 0; x2 = 15; y1= 3; y2 = 3;
	print_cuadrado(buffer, (C_FG_BLACK | C_BG_GREEN), x1, y1, x2, y2);

	//cambiar_pantalla(SCREENMAPA);
	//load_pantalla();

	/*cambiar_pantalla(SCREENMAPA);
	load_pantalla();*/
}

//
// Funciones de pantalla
//

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

///
/// Prints
///

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

void print_texto(screen * pantalla, char * texto, int x, int y) {
	pixel pix;
	int z = 0;
	while(texto[z] != 0){
		pix.letra = texto[z];
		pix.formato = ((* pantalla)[pos(x+z,y)]).formato;
		print_pixel(pantalla, pix, x+z, y);
		z ++;
	}
}

void print_formato(screen * pantalla, char  formato, int x, int y) {
	(* pantalla)[pos(x,y)].formato = formato;
}

void print_pixel(screen * pantalla, pixel pix, int x, int y){	(*pantalla)[pos(x,y)] = pix;}


///
/// MISC
///

int pos(int x, int y){return (x+ (y*80));};

void reg_a_string(int registro, char * txt, int inicio){
	int posicion = 7 + inicio; //voy de atras para adelante
	int valor;
	while(posicion >= inicio){
		valor = registro & 0xF;
		if(valor < 10){ valor += ASCII_first_num;
		}else{ valor -= 10; valor += ASCII_first_let;
		};
		txt[posicion] = valor;
		registro = registro >> 4;
		posicion--;
	}
};


/////
///// PRINTS ESPECIFICOS
/////

void print_error(char error){

};

void print_tablaerror(int eax, int ebx, int ecx, int edx, int esi, 
	int edi, int ebp, int esp, int eip, int cr0, int cr2, int cr3 ,
	int cs, int ds, int es, int fs, int gs, int ss, int eflags){
	char string[] = "??? ????????";
	char eflags_string[] = "EFLAGS";
	// PRIMERA COLUMNA
		int x = tablaerror_x;	int y = tablaerror_y;
		/*eax*/  string[0] = *(char *) "e"; string[2] = *(char *) "x";
		string[1] = *(char *) "a";	reg_a_string(eax, string, 4);
		print_texto(ESTADO, string, x, y);
		/*ebx*/	y += 1 ; string[1] = *(char *) "b";	reg_a_string(ebx, string, 4);
		print_texto(ESTADO, string, x, y);
		/*ecx*/	y += 1 ; string[1] = *(char *) "c";	reg_a_string(ecx, string, 4);
		print_texto(ESTADO, string, x, y);
		/*edx*/ y += 1 ; string[1] = *(char *) "d";	reg_a_string(edx, string, 4);
		print_texto(ESTADO, string, x, y);
		/*esi*/ string[2] = *(char *) "i"; string[1] = *(char *) "s";
		y += 1; reg_a_string(esi, string, 4);
		print_texto(ESTADO, string, x, y);
		/*edi*/ string[1] = *(char *) "d"; y += 1; reg_a_string(esi, string, 4);
		print_texto(ESTADO, string, x, y);
		/*ebp*/ string[2] = *(char *) "p"; string[1] = *(char *) "b";
		y += 1; reg_a_string(ebp, string, 4);
		print_texto(ESTADO, string, x, y);
		/*esp*/ string[1] = *(char *) "s"; y += 1; reg_a_string(esp, string, 4);
		print_texto(ESTADO, string, x, y);
		/*eip*/ string[1] = *(char *) "i"; y += 1; reg_a_string(eip, string, 4);
		print_texto(ESTADO, string, x, y);
		/*CR0*/  string[0] = *(char *) "C"; string[1] = *(char *) "R";
		string[2] = *(char *) "0";  y+=1;reg_a_string(cr0, string, 4);
		print_texto(ESTADO, string, x, y);
		/*CR2*/string[2] = *(char *) "2";  y+=1;reg_a_string(cr2, string, 4);
		print_texto(ESTADO, string, x, y);
		/*CR3*/string[2] = *(char *) "3";  y+=1;reg_a_string(cr3, string, 4);
		print_texto(ESTADO, string, x, y);
	// SEGUNDA COLUMNA
		x = tablaerror_x + 14;	y = tablaerror_y;
		/*?s*/ string[1] = *(char *) "s"; string[2] = *(char *) " ";
		/*cs*/ string[0] = *(char *) "c"; reg_a_string(cs, string, 4);
		print_texto(ESTADO, string, x, y); y++;
		/*ds*/ string[0] = *(char *) "d"; reg_a_string(ds, string, 4);
		print_texto(ESTADO, string, x, y); y++;
		/*es*/ string[0] = *(char *) "e"; reg_a_string(es, string, 4);
		print_texto(ESTADO, string, x, y); y++;
		/*fs*/ string[0] = *(char *) "f"; reg_a_string(fs, string, 4);
		print_texto(ESTADO, string, x, y); y++;
		/*gs*/ string[0] = *(char *) "g"; reg_a_string(gs, string, 4);
		print_texto(ESTADO, string, x, y); y++;
		/*ss*/ string[0] = *(char *) "s"; reg_a_string(ss, string, 4);
		print_texto(ESTADO, string, x, y); y++;
		y++; 
		/*EFLGAS*/
		print_texto(ESTADO, eflags_string, x, y);
		y++;
		string[1] = *(char *) " "; string[0] = *(char *) " ";
		reg_a_string(eflags, string, 4);
		print_texto(ESTADO, string, x, y);
}

void print_tablatar(int tarea, int pg1, int pg2, int pg3){
	int x = tablatar_x + 1;
	int y = tablatar_y + tarea - 1;
	char string[] = "p?:0x????????";
	//pg1
	string[1] = *(char *) "1"; reg_a_string(pg1, string, 5);
	print_texto(ESTADO, string, x, y); x+= 14;
	//pg2
	string[1] = *(char *) "2"; reg_a_string(pg2, string, 5);
	print_texto(ESTADO, string, x, y); x+= 14;
	//pg3
	string[1] = *(char *) "3"; reg_a_string(pg3, string, 5);
	print_texto(ESTADO, string, x, y);
}

void print_tablatar_from_gdt(int tarea){
	int pg0 = get_pagina_fisica(tarea,0);
	int pg1 = get_pagina_fisica(tarea,1);
	int pg2 = get_pagina_fisica(tarea,2);
	print_tablatar(tarea,pg0,pg1,pg2);

};


void print_tablatar_error(int tarea, int num_error){
	int x = tablatar_x;
	int y = tablatar_y + tarea - 1;
	while (x <= 78){
		print_formato(ESTADO, (C_FG_BLACK | C_BG_BROWN),  x, y);
		x++;
	}
	x -= 12;
	print_texto(ESTADO,string_errores[num_error], x, y);
}