#include "screen.h"
#include "colors.h"
#include "defines.h"
extern unsigned int TASK_PAG_1[];
extern unsigned int TASK_PAG_2[];
extern unsigned int TASK_PAG_3[];
extern struct sched_t sched;

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

	print_cuadrado(DISPLAY, (C_FG_WHITE | C_BG_BLACK),0, 24, 79, 24);

	//////
	/// GENERO BUFFER ESTADO
	/////
	screen* buffer = ESTADO;

	//pongo todo de blanco
	pixel_actual.letra = *((char *) " ");
	pixel_actual.formato = (C_FG_BLACK | C_BG_CYAN);
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
	print_cuadrado(buffer, (C_FG_LIGHT_BROWN | C_BG_BLUE),x1,y1,x2,y2);
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
	print_cuadrado(buffer, (C_FG_WHITE | C_BG_BLUE), x1, y1, x2, y2);

	// Imprime ejemplo de errores
	//int b = 0xA1020B01;
	///print_tablaerror(b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b);

	// Imprime ejemplo tareas
	
	

	/*char *stringz = "        ";
	reg_a_string(dir_a_cord(get_pagina_fisica(1,0)), stringz, 0);
	print_texto(ESTADO, stringz, 0, 1);*/
	

	cambiar_pantalla(SCREENESTADO);
	load_pantalla();
	//////
	/// GENERO BUFFER MAPA
	/////
	buffer = MAPA;
	x1 = 0; x2 = 79; y1= 0; y2 = 2;
	print_cuadrado(buffer, (C_FG_BLACK | C_BG_GREEN), x1, y1, x2, y2);
	x1 = 0; x2 = 79; y1= 3; y2 = 24;
	print_cuadrado(buffer, (C_FG_RED | C_BG_BLUE), x1, y1, x2, y2);
	x1 = 0; x2 = 15; y1= 3; y2 = 3;
	print_cuadrado(buffer, (C_FG_BLACK | C_BG_GREEN), x1, y1, x2, y2);

	print_missil(0x10000);
	print_missil_cord(256);
	print_missil_cord(257);
	print_missil_cord(258);

	//cambiar_pantalla(SCREENMAPA);
	load_pantalla();
	print_banderines();
	//cambiar_pantalla(SCREENMAPA);
	load_pantalla();
}

void inicializar_pantalla_memoria(){ //NECESITA QUE ESTE DEFINIDA LA GDT PARA FUNCIONAR
	print_tablatar_tarea(1);
	print_tablatar_tarea(2);
	print_tablatar_tarea(3);
	print_tablatar_tarea(4);
	print_tablatar_tarea(5);
	print_tablatar_tarea(6);
	print_tablatar_tarea(7);
	print_tablatar_tarea(8);

	//print_tablatar_error(3,10); //ESTO NO IRIA

	print_mapa_tarea(1);
	print_mapa_tarea(2);
	print_mapa_tarea(3);
	print_mapa_tarea(4);
	print_mapa_tarea(5);
	print_mapa_tarea(6);
	print_mapa_tarea(7);
	print_mapa_tarea(8);

	load_pantalla();

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


void print_texto_cord(screen * pantalla, char * texto, int cordenada) {
	pixel pix;
	int z = 0;
	while(texto[z] != 0){
		pix.letra = texto[z];
		pix.formato = ((* pantalla)[cordenada+z]).formato;
		print_pixel_cord(pantalla, pix, cordenada+z);
		z ++;
	}
}
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


void print_formato_cord(screen * pantalla, char  formato, int cordenada) {
	(* pantalla)[cordenada].formato = formato;
}
void print_formato(screen * pantalla, char  formato, int x, int y) {
	(* pantalla)[pos(x,y)].formato = formato;
}

void print_pixel_cord(screen * pantalla, pixel pix, int cordenada){	(*pantalla)[cordenada] = pix;}
void print_pixel(screen * pantalla, pixel pix, int x, int y){	(*pantalla)[pos(x,y)] = pix;}



/////
///// PRINTS ESPECIFICOS
/////

////// Tabla Error

void print_error(char error){
	char * string1 = &(string_errores[(unsigned char)error][0]);
	char string2[] = "                NAVIO ?";
	unsigned int number = 22;
	int i = 0;
	while((* string1) != 0){
		string2[i] = (* string1);
		string1 ++;	i++;
	}
	string2[number] = sched.TAREA_ACTUAL + ASCII_first_num;
	print_texto(ESTADO, string2,tablaerror_x,tablaerror_y -1);
};

void print_tablaerror(){
	char string[] = "??? ????????";
	char eflags_string[] = "EFLAGS";
	// PRIMERA COLUMNA
		int x = tablaerror_x;	int y = tablaerror_y;
		/*eax*/  string[0] = *(char *) "e"; string[2] = *(char *) "x";
		string[1] = *(char *) "a";	reg_a_string(estado_error.eax, string, 4);
		print_texto(ESTADO, string, x, y);
		/*ebx*/	y += 1 ; string[1] = *(char *) "b";	reg_a_string(estado_error.ebx, string, 4);
		print_texto(ESTADO, string, x, y);
		/*ecx*/	y += 1 ; string[1] = *(char *) "c";	reg_a_string(estado_error.ecx, string, 4);
		print_texto(ESTADO, string, x, y);
		/*edx*/ y += 1 ; string[1] = *(char *) "d";	reg_a_string(estado_error.edx, string, 4);
		print_texto(ESTADO, string, x, y);
		/*esi*/ string[2] = *(char *) "i"; string[1] = *(char *) "s";
		y += 1; reg_a_string(estado_error.esi, string, 4);
		print_texto(ESTADO, string, x, y);
		/*edi*/ string[1] = *(char *) "d"; y += 1; reg_a_string(estado_error.edi, string, 4);
		print_texto(ESTADO, string, x, y);
		/*ebp*/ string[2] = *(char *) "p"; string[1] = *(char *) "b";
		y += 1; reg_a_string(estado_error.ebp, string, 4);
		print_texto(ESTADO, string, x, y);
		/*esp*/ string[1] = *(char *) "s"; y += 1; reg_a_string(estado_error.esp, string, 4);
		print_texto(ESTADO, string, x, y);
		/*eip*/ string[1] = *(char *) "i"; y += 1; reg_a_string(estado_error.eip, string, 4);
		print_texto(ESTADO, string, x, y);
		/*CR0*/  string[0] = *(char *) "C"; string[1] = *(char *) "R";
		string[2] = *(char *) "0";  y+=1;reg_a_string(estado_error.cr0, string, 4);
		print_texto(ESTADO, string, x, y);
		/*CR2*/string[2] = *(char *) "2";  y+=1;reg_a_string(estado_error.cr2, string, 4);
		print_texto(ESTADO, string, x, y);
		/*CR3*/string[2] = *(char *) "3";  y+=1;reg_a_string(estado_error.cr3, string, 4);
		print_texto(ESTADO, string, x, y);
	// SEGUNDA COLUMNA
		x = tablaerror_x + 14;	y = tablaerror_y;
		/*?s*/ string[1] = *(char *) "s"; string[2] = *(char *) " ";
		/*cs*/ string[0] = *(char *) "c"; reg_a_string(estado_error.cs, string, 4);
		print_texto(ESTADO, string, x, y); y++;
		/*ds*/ string[0] = *(char *) "d"; reg_a_string(estado_error.ds, string, 4);
		print_texto(ESTADO, string, x, y); y++;
		/*es*/ string[0] = *(char *) "e"; reg_a_string(estado_error.es, string, 4);
		print_texto(ESTADO, string, x, y); y++;
		/*fs*/ string[0] = *(char *) "f"; reg_a_string(estado_error.fs, string, 4);
		print_texto(ESTADO, string, x, y); y++;
		/*gs*/ string[0] = *(char *) "g"; reg_a_string(estado_error.gs, string, 4);
		print_texto(ESTADO, string, x, y); y++;
		/*ss*/ string[0] = *(char *) "s"; reg_a_string(estado_error.ss, string, 4);
		print_texto(ESTADO, string, x, y); y++;
		y++; 
		/*EFLGAS*/
		print_texto(ESTADO, eflags_string, x, y);
		y++;
		string[1] = *(char *) " "; string[0] = *(char *) " ";
		reg_a_string(estado_error.eflags, string, 4);
		print_texto(ESTADO, string, x, y);

		//load_pantalla();
}

/////// Tabla Tarea

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

void print_tablatar_tarea(int tarea){
	int pg0 = TASK_PAG_1[tarea];
	int pg1 = TASK_PAG_2[tarea];
	int pg2 = TASK_PAG_3[tarea];
	print_tablatar(tarea,pg0,pg1,pg2);

};

void print_tablatar_error(int tarea, char* string){
	int x = tablatar_x;
	int y = tablatar_y + tarea - 1;
	while (x <= 78){
		print_formato(ESTADO, (C_FG_BLACK | C_BG_BROWN),  x, y);
		x++;
	}
	x -= 15;
	print_texto(ESTADO,string, x, y);
}

void print_tablatar_int(int tarea, int num_error){
	char * string = string_errores[num_error];
	print_tablatar_error(tarea, string);
}

void print_tablatar_int_actual(int num_error){print_tablatar_int(sched.TAREA_ACTUAL, num_error);}

////// Mapa 

void print_mapa_tarea(int tarea){
	int pg0 = TASK_PAG_1[tarea];
	int pg1 = TASK_PAG_2[tarea];
	int pg2 = TASK_PAG_3[tarea];
	print_pg_en_mapa(tarea, pg0);
	print_pg_en_mapa(tarea, pg1);
	print_pg_en_mapa(tarea, pg2);
}

void print_numero_mapa_cord(int cordenada){
	pixel pix; char dato = (* map_uses)[cordenada];
	pix.formato = (C_FG_BLACK | C_BG_RED);
	if (dato == 0){	
		pix.letra = ASCII_space; 			//SI ES 0 NO IMPRIMO NADA
		if(cordenada <= 256){ 	pix.formato = (C_FG_BLACK | C_BG_GREEN);}
		else{					pix.formato = (C_FG_BLACK | C_BG_BLUE);}						
	} 	
	else if(dato < 9){	
		pix.letra = dato + ASCII_first_num; //SI ES UN NUMERO LO IMPRIMO
	}else{	
		pix.letra = *(char *)"X"; 			//SI SUPERA LA CANTIDAD DE TAREAS IMPRIMO X			
	} 
	print_pixel_cord(MAPA, pix, cordenada);
};

void print_numero_mapa(int x, int y){print_numero_mapa_cord(pos(x,y));};

void unprint_mapa_tarea(int tarea){
	unprint_pg_mapa(tarea,0);
	unprint_pg_mapa(tarea,1);
	unprint_pg_mapa(tarea,2);
}

void unprint_pg_mapa(int tarea, int pagina){
	int pgdir; 
	if(pagina == 0){pgdir = TASK_PAG_1[tarea];}
	if(pagina == 1){pgdir = TASK_PAG_2[tarea];}
	if(pagina == 2){pgdir = TASK_PAG_3[tarea];}
	int pgcord = dir_a_cord(pgdir);
	char uso = (* map_uses)[pgcord];
	if(uso > 0 ){ //Este IF nos permite unprint si no hubo prints
		uso -= tarea; if(uso > 10){uso -= 10;}; 
	};	
	(* map_uses)[pgcord] = uso;
	print_numero_mapa_cord(pgcord);
}

void print_pg_en_mapa(int tarea, int direccion){
	int cordenada = dir_a_cord(direccion);
	char uso = (* map_uses)[cordenada];
	if( uso != 0) { uso += 10;} uso += tarea;
	(* map_uses)[cordenada] = uso;
	print_numero_mapa_cord(cordenada);
}

void print_missil(int direccion){ print_missil_cord(dir_a_cord(direccion));}

void print_missil_cord(int cordenada){
	int ult_cordenada 	= (int) (ultimo_missil); 
	ult_cordenada 	   -= (int) 		 (MAPA);
	ult_cordenada      /=  					  2;
	print_numero_mapa_cord(ult_cordenada);
	
	ultimo_missil = &((* MAPA)[cordenada]);
	(* ultimo_missil).formato = (C_FG_BLACK | C_BG_BROWN);
	(* ultimo_missil).letra = * (char*) "#";

};

void print_bandera(int tarea){
	//tarea = sched.TAREA_ACTUAL;
	char * origen = (char *) (0x40001000);
	////char:char* origen = &ejemplo_bandera2[0];
	/*pix*/	//char* origen = (char *) &ejemplo_bandera[0];
	//char: no iria nada
	/*pix*///
	//origen ++;
	pixel pix;
	pix.letra = *(char *)" ";
	int y = 0; int x;
	while(y < 5){
		x = 0;
		while(x< 10){
			pix.letra = ((*origen));
			origen++;
			pix.formato = ((*origen));
			origen++;  
			//pix.formato = 0XF;
			//char: 
			//origen+= 2;
			/*pix*///
			//print_pixel(ESTADO, pix, bandera_x[tarea-1] + x, bandera_y[tarea-1] + y);
			print_pixel(ESTADO, pix, bandera_x[tarea-1] + x, bandera_y[tarea-1] + y);
			x++;
		}
		y++;
	};
	load_pantalla();
}

void print_banderines(){
	int y = 20;
	int x = 3;
	screen* pantalla = DISPLAY;
	pixel pix1;
	pixel pix2;
	//
	char s0[] =   "[ ----- ]";
	char s1_2[] = "[ IDLE  ]";
	char s3[] =   "[ TAREA ]";
	char s4[] =   "[ FLAG  ]";
	char * answer;
	switch(sched.CONTEXTO){
		case 0:
			answer = &s0[0];
			break;
		case 1:
		case 2:
			answer = &s1_2[0];
			break;

		case 3:
			answer = &s3[0];
			break;
		case 4:
			answer = &s4[0];
			break;
		default:
			answer = &s0[0];
			break;
	}
	print_texto(DISPLAY, answer, 3, 24);
	//
	int z = 1;
	x = x + 9 + 2;
	y = 24;
	while(z <= 8){
		pix1.formato = (C_FG_WHITE | C_BG_BLACK);
		//pix1.formato = (C_FG_WHITE | C_BG_GREEN);
		pix2.formato = (C_FG_WHITE | C_BG_BLACK);
		//pix2.formato = (C_FG_WHITE | C_BG_CYAN);
		if(sched.tareas[z].estado == 0){
			pix1.formato = (C_FG_DARK_GREY | C_BG_BLACK);
			pix2.formato = (C_FG_DARK_GREY | C_BG_BLACK);
		}else{ 
			if(sched.TAREA_ACTUAL == z){
				pix1.formato = (C_FG_WHITE | C_BG_GREEN);
			}

			if(sched.BANDERA_ACTUAL == z){
				pix2.formato = (C_FG_WHITE | C_BG_CYAN);
			}
		}
		// PRINT TAREA
		pix1.letra = *(char*)"T";
		print_pixel(pantalla, pix1, x, y);
		pix1.letra = ASCII_first_num + z;
		print_pixel(pantalla, pix1, x+1, y);
		//PRINT FLAG
		pix2.letra = *(char*)"F";
		print_pixel(pantalla, pix2, x+3, y);
		pix2.letra = ASCII_first_num + z;
		print_pixel(pantalla, pix2, x+4, y);

		x += 7;
		z++;
	}
}
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

int dir_a_cord(int dir_fisica){return dir_fisica/ 0x1000;};
int cord_a_dir(int cordenada ){return cordenada * 0x1000;};