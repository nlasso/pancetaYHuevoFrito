#include <getopt.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "filtros.h"
#include "tiempo.h"
#include "utils.h"

const char* nombre_programa;

void imprimir_ayuda ( );
void imprimir_tiempos_ejecucion(unsigned long long int start, unsigned long long int end, int cant_iteraciones);

void aplicar_recortar	(int tiempo, int cant_iteraciones, const char *nomb_impl, const char *nomb_arch_entrada, int tam);
void aplicar_colorizar	(int tiempo, int cant_iteraciones, const char *nomb_impl, const char *nomb_arch_entrada, float alpha);
void aplicar_halftone	(int tiempo, int cant_iteraciones, const char *nomb_impl, const char *nomb_arch_entrada);
void aplicar_rotar		(int tiempo, int cant_iteraciones, const char *nomb_impl, const char *nomb_arch_entrada);
void aplicar_umbralizar	(int tiempo, int cant_iteraciones, const char *nomb_impl, const char *nomb_arch_entrada, unsigned char min, unsigned char max, unsigned char q);
void aplicar_waves		(int tiempo, int cant_iteraciones, const char *nomb_impl, const char *nomb_arch_entrada, float x_scale, float y_scale, float g_scale);

int main( int argc, char** argv ) {
	int siguiente_opcion;

	// Opciones
	const char* const op_cortas = "hi:vt:";

	const struct option op_largas[] = {
		{ "help", 0, NULL, 'h' },
		{ "implementacion", 1, NULL, 'i' },
		{ "verbose", 0, NULL, 'v' },
		{ "tiempo", 1, NULL, 't' },
		{ NULL, 0, NULL, 0 }
	};

	// Parametros
	const char* nombre_implementacion = NULL;
	int cant_iteraciones = 0;

	// Flags de opciones
	int verbose = 0;
	int tiempo = 0;

	// Guardar nombre del programa para usarlo en la ayuda
	nombre_programa = argv[0];

	// Si se ejecuta sin parametros ni opciones
	if (argc == 1) {
		imprimir_ayuda ( );

		exit ( EXIT_SUCCESS );
	}

	// Procesar opciones
	while (1) {
		siguiente_opcion = getopt_long ( argc, argv, op_cortas, op_largas, NULL);

		// No hay mas opciones
		if ( siguiente_opcion == -1 )
			break;

		// Procesar opcion
		switch ( siguiente_opcion ) {
			case 'h' : /* -h o --help */
				imprimir_ayuda ( );
				exit ( EXIT_SUCCESS );
				break;
			case 'i' : /* -i o --implementacion */
				nombre_implementacion = optarg;
				break;
			case 't' : /* -t o --tiempo */
				tiempo = 1;
				cant_iteraciones = atoi ( optarg );
				break;
			case 'v' : /* -v o --verbose */
				verbose = 1;
				break;
			case '?' : /* opcion no valida */
				imprimir_ayuda ( );
				exit ( EXIT_SUCCESS );
			default : /* opcion no valida */
				abort ( );
		}
	}

	// Verifico nombre del proceso
	char *nomb_proceso = argv[optind++];

	if (nomb_proceso == NULL ||
		(strcmp(nomb_proceso, "recortar") != 0			&&
		strcmp(nomb_proceso, "colorizar") != 0			&&
		strcmp(nomb_proceso, "halftone") != 0			&&
		strcmp(nomb_proceso, "rotar") != 0				&&
		strcmp(nomb_proceso, "umbralizar") != 0			&&
		strcmp(nomb_proceso, "waves") != 0)) {
		imprimir_ayuda ( );

		exit ( EXIT_SUCCESS );
	}

	// Verifico nombre de la implementacion
	if (nombre_implementacion == NULL ||
		(strcmp(nombre_implementacion, "c") != 0 &&
		strcmp(nombre_implementacion, "asm") != 0)) {
		imprimir_ayuda ( );

		exit ( EXIT_SUCCESS );
	}

	// Verifico nombre de archivo
	const char *nomb_arch_entrada = argv[optind++];

	if (nomb_arch_entrada == NULL) {
		imprimir_ayuda ( );

		exit ( EXIT_SUCCESS );
	}

	if (access( nomb_arch_entrada, F_OK ) == -1) {
		printf("Error al intentar abrir el archivo: %s.\n", nomb_arch_entrada);

		exit ( EXIT_SUCCESS );
	}

	// Imprimo info
	if ( verbose ) {
		printf ( "Procesando imagen...\n");
		printf ( "  Filtro             : %s\n", nomb_proceso);
		printf ( "  Implementación     : %s\n", nombre_implementacion);
		printf ( "  Archivo de entrada : %s\n", nomb_arch_entrada);
	}

	// Procesar imagen
	if (strcmp(nomb_proceso, "recortar") == 0)
	{
		int tam = atoi(argv[optind++]);

		aplicar_recortar(tiempo, cant_iteraciones, nombre_implementacion,
			nomb_arch_entrada, tam);
	}
	else if (strcmp(nomb_proceso, "colorizar") == 0)
	{
		float alpha = (float) atof(argv[optind++]);

		aplicar_colorizar(tiempo, cant_iteraciones, nombre_implementacion, nomb_arch_entrada, alpha);
	}
	else if (strcmp(nomb_proceso, "halftone") == 0)
	{
		aplicar_halftone(tiempo, cant_iteraciones, nombre_implementacion, nomb_arch_entrada);
	}
	else if (strcmp(nomb_proceso, "rotar") == 0)
	{
		aplicar_rotar(tiempo, cant_iteraciones, nombre_implementacion, nomb_arch_entrada);
	}
	else if (strcmp(nomb_proceso, "umbralizar") == 0)
	{
		int min = atoi(argv[optind++]);
		int max = atoi(argv[optind++]);
		int q = atoi(argv[optind++]);

		aplicar_umbralizar(tiempo, cant_iteraciones, nombre_implementacion,
			nomb_arch_entrada, min, max, q);
	}
	else if (strcmp(nomb_proceso, "waves") == 0)
	{
		float x_scale = (float) atof(argv[optind++]);
		float y_scale = (float) atof(argv[optind++]);
		float g_scale = (float) atof(argv[optind++]);

		aplicar_waves(tiempo, cant_iteraciones, nombre_implementacion,
			nomb_arch_entrada, x_scale, y_scale, g_scale);
	}

	return 0;
}

void imprimir_ayuda ( ) {
	printf ( "Uso: %s opciones filtro nombre_archivo_entrada parametros_filtro                                            \n", nombre_programa );
	printf ( "    Los filtros que se pueden aplicar son:                                                                  \n" );
	printf ( "       * recortar                                                                                           \n" );
	printf ( "           Parámetros     : tamaño                                                                          \n" );
	printf ( "           Ejemplo de uso : %s -i c recortar lena.bmp 100                                                   \n", nombre_programa );
	printf ( "       * colorizar                                                                                          \n" );
	printf ( "           Parámetros     : alpha : [0.0,  1.0]                                                             \n" );
	printf ( "           Ejemplo de uso : %s -i c colorizar lena.bmp 0.5                                                  \n", nombre_programa );
	printf ( "       * halftone                                                                                           \n" );
	printf ( "       * rotar                                                                                              \n" );
	printf ( "       * umbralizar                                                                                         \n" );
	printf ( "           Parámetros     : min : [0, 255]                                                                  \n" );
	printf ( "                            max : [0, 255]                                                                  \n" );
	printf ( "                            q   : [0, 255]                                                                  \n" );
	printf ( "           Ejemplo de uso : %s -i c umbralizar lena.bmp 64 128 16                                           \n", nombre_programa );
	printf ( "       * waves                                                                                              \n" );
	printf ( "           Parámetros     : x_scale : [0.0,  32.0]                                                          \n" );
	printf ( "                            y_scale : [0.0,  32.0]                                                          \n" );
	printf ( "                            g_scale : [0.0, 255.0]                                                          \n" );
	printf ( "           Ejemplo de uso : %s -i c recortar lena.bmp 2.0 4.0 16.0                                          \n", nombre_programa );
	printf ( "                                                                                                            \n" );
	printf ( "    -h, --help                                 Imprime esta ayuda                                           \n" );
	printf ( "                                                                                                            \n" );
	printf ( "    -i, --implementacion NOMBRE_MODO           Implementación sobre la que se ejecutará el filtro           \n" );
	printf ( "                                               seleccionado. Los implementaciones disponibles               \n" );
	printf ( "                                               son: c, asm                                                  \n" );
	printf ( "                                                                                                            \n" );
	printf ( "    -t, --tiempo CANT_ITERACIONES              Mide el tiempo que tarda en ejecutar el filtro sobre la      \n" );
	printf ( "                                               imagen de entrada una cantidad de veces igual a              \n" );
	printf ( "                                               CANT_ITERACIONES                                             \n" );
	printf ( "                                                                                                            \n" );
	printf ( "    -v, --verbose                              Imprime información adicional                                \n" );
	printf ( "                                                                                                            \n" );
}

void imprimir_tiempos_ejecucion(unsigned long long int start, unsigned long long int end, int cant_iteraciones) {
	unsigned long long int cant_ciclos = end-start;

	printf("Tiempo de ejecución:\n");
	printf("  Comienzo                          : %llu\n", start);
	printf("  Fin                               : %llu\n", end);
	printf("  # iteraciones                     : %d\n", cant_iteraciones);
	printf("  # de ciclos insumidos totales     : %llu\n", cant_ciclos);
	printf("  # de ciclos insumidos por llamada : %.3f\n", (float)cant_ciclos/(float)cant_iteraciones);
}

void aplicar_recortar (int tiempo, int cant_iteraciones, const char *nomb_impl, const char *nomb_arch_entrada, int tam) {
	IplImage *src = 0;
	IplImage *dst = 0;
	CvSize dst_size;

	// Cargo la imagen
	if( (src = cvLoadImage (nomb_arch_entrada, CV_LOAD_IMAGE_GRAYSCALE)) == 0 )
		exit(EXIT_FAILURE);

	dst_size.width = tam * 2;
	dst_size.height = tam * 2;

	// Chequeo de parametros
	if (!(tam < src->width || tam < src->height)) {
		imprimir_ayuda();

		cvReleaseImage(&src);

		exit ( EXIT_SUCCESS );
	}

	// Creo una IplImage para cada salida esperada
	if( (dst = cvCreateImage (dst_size, IPL_DEPTH_8U, 1) ) == 0 )
		exit(EXIT_FAILURE);

	typedef void (recortar_fn_t) (unsigned char*, unsigned char*, int, int, int, int, int);

	recortar_fn_t *proceso;

	if (strcmp(nomb_impl, "c") == 0) {
		proceso = recortar_c;
	} else {
		proceso = recortar_asm;
	}

	if (tiempo) {
		unsigned long long int start, end;

		MEDIR_TIEMPO_START(start);

		for(int i=0; i<cant_iteraciones; i++) {
			proceso((unsigned char*)src->imageData, (unsigned char*)dst->imageData, src->height, src->width, src->widthStep, dst->widthStep, tam);
		}

		MEDIR_TIEMPO_STOP(end);

		imprimir_tiempos_ejecucion(start, end, cant_iteraciones);
	} else {
		proceso((unsigned char*)src->imageData, (unsigned char*)dst->imageData, src->height, src->width, src->widthStep, dst->widthStep, tam);
	}

	// Guardo imagen y libero las imagenes
	char nomb_arch_salida[256];

	memset(nomb_arch_salida, 0, 256);

	sprintf(nomb_arch_salida, "%s.recortar.tam-%d.%s.bmp", nomb_arch_entrada, tam, nomb_impl);

	cvSaveImage(nomb_arch_salida, dst, NULL);

	cvReleaseImage(&src);
	cvReleaseImage(&dst);
}


void aplicar_colorizar (int tiempo, int cant_iteraciones, const char *nomb_impl, const char *nomb_arch_entrada, float alpha) {
	IplImage *src = 0;
	IplImage *dst = 0;
	CvSize dst_size;

	// Cargo la imagen
	if( (src = cvLoadImage (nomb_arch_entrada, CV_LOAD_IMAGE_COLOR)) == 0 )
		exit(EXIT_FAILURE);

	dst_size.width = src->width;
	dst_size.height = src->height;


	// Creo una IplImage para cada salida esperada
	if( (dst = cvCreateImage (dst_size, IPL_DEPTH_8U, 3) ) == 0 )
		exit(EXIT_FAILURE);

	typedef void (colorizar_fn_t) (unsigned char*, unsigned char*, int, int, int, int, float);

	colorizar_fn_t *proceso;

	if (strcmp(nomb_impl, "c") == 0) {
		proceso = colorizar_c;
	} else {
		proceso = colorizar_asm;
	}

	if (tiempo) {
		unsigned long long int start, end;

		MEDIR_TIEMPO_START(start);

		for(int i=0; i<cant_iteraciones; i++) {
			proceso((unsigned char*)src->imageData, (unsigned char*)dst->imageData, src->height, src->width, src->widthStep, dst->widthStep, alpha);
		}

		MEDIR_TIEMPO_STOP(end);

		imprimir_tiempos_ejecucion(start, end, cant_iteraciones);
	} else {
		proceso((unsigned char*)src->imageData, (unsigned char*)dst->imageData, src->height, src->width, src->widthStep, dst->widthStep, alpha);
	}

	copiar_bordes_color((unsigned char*)src->imageData, (unsigned char*)dst->imageData, src->height, src->width, src->widthStep);

	// Guardo imagen y libero las imagenes
	char nomb_arch_salida[256];

	memset(nomb_arch_salida, 0, 256);

	sprintf(nomb_arch_salida, "%s.colorizar.alpha-%3.2f.%s.bmp", nomb_arch_entrada, alpha, nomb_impl);

	cvSaveImage(nomb_arch_salida, dst, NULL);

	cvReleaseImage(&src);
	cvReleaseImage(&dst);
}


void aplicar_halftone (int tiempo, int cant_iteraciones, const char *nomb_impl, const char *nomb_arch_entrada) {
	IplImage *src = 0;
	IplImage *dst = 0;
	CvSize dst_size;

	// Cargo la imagen
	if( (src = cvLoadImage (nomb_arch_entrada, CV_LOAD_IMAGE_GRAYSCALE)) == 0 )
		exit(EXIT_FAILURE);

	dst_size.width = src->width - src->width % 2;
	dst_size.height = src->height - src->height % 2;

	// Creo una IplImage para cada salida esperada
	if( (dst = cvCreateImage (dst_size, IPL_DEPTH_8U, 1) ) == 0 )
		exit(EXIT_FAILURE);

	typedef void (halftone_fn_t) (unsigned char*, unsigned char*, int, int, int, int);

	halftone_fn_t *proceso;

	if (strcmp(nomb_impl, "c") == 0) {
		proceso = halftone_c;
	} else {
		proceso = halftone_asm;
	}

	if (tiempo) {
		unsigned long long int start, end;

		MEDIR_TIEMPO_START(start);

		for(int i=0; i<cant_iteraciones; i++) {
			proceso((unsigned char*)src->imageData, (unsigned char*)dst->imageData, src->height, src->width, src->widthStep, dst->widthStep);
		}

		MEDIR_TIEMPO_STOP(end);

		imprimir_tiempos_ejecucion(start, end, cant_iteraciones);
	} else {
		proceso((unsigned char*)src->imageData, (unsigned char*)dst->imageData, src->height, src->width, src->widthStep, dst->widthStep);
	}

	// Guardo imagen y libero las imagenes
	char nomb_arch_salida[256];

	memset(nomb_arch_salida, 0, 256);

	sprintf(nomb_arch_salida, "%s.halftone.%s.bmp", nomb_arch_entrada, nomb_impl);

	cvSaveImage(nomb_arch_salida, dst, NULL);

	cvReleaseImage(&src);
	cvReleaseImage(&dst);
}


void aplicar_rotar (int tiempo, int cant_iteraciones, const char *nomb_impl, const char *nomb_arch_entrada) {
	IplImage *src = 0;
	IplImage *dst = 0;
	CvSize dst_size;

	// Cargo la imagen
	if( (src = cvLoadImage (nomb_arch_entrada, CV_LOAD_IMAGE_GRAYSCALE)) == 0 )
		exit(EXIT_FAILURE);

	dst_size.width = src->width;
	dst_size.height = src->height;

	// Creo una IplImage para cada salida esperada
	if( (dst = cvCreateImage (dst_size, IPL_DEPTH_8U, 1) ) == 0 )
		exit(EXIT_FAILURE);

	typedef void (rotar_fn_t) (unsigned char*, unsigned char*, int, int, int, int);

	rotar_fn_t *proceso;

	if (strcmp(nomb_impl, "c") == 0) {
		proceso = rotar_c;
	} else {
		proceso = rotar_asm;
	}

	if (tiempo) {
		unsigned long long int start, end;

		MEDIR_TIEMPO_START(start);

		for(int i=0; i<cant_iteraciones; i++) {
			proceso((unsigned char*)src->imageData, (unsigned char*)dst->imageData, src->height, src->width, src->widthStep, dst->widthStep);
		}

		MEDIR_TIEMPO_STOP(end);

		imprimir_tiempos_ejecucion(start, end, cant_iteraciones);
	} else {
		proceso((unsigned char*)src->imageData, (unsigned char*)dst->imageData, src->height, src->width, src->widthStep, dst->widthStep);
	}

	// Guardo imagen y libero las imagenes
	char nomb_arch_salida[256];

	memset(nomb_arch_salida, 0, 256);

	sprintf(nomb_arch_salida, "%s.rotar.%s.bmp", nomb_arch_entrada, nomb_impl);

	cvSaveImage(nomb_arch_salida, dst, NULL);

	cvReleaseImage(&src);
	cvReleaseImage(&dst);
}


void aplicar_umbralizar (int tiempo, int cant_iteraciones, const char *nomb_impl, const char *nomb_arch_entrada, unsigned char min, unsigned char max, unsigned char q) {
	IplImage *src = 0;
	IplImage *dst = 0;
	CvSize dst_size;

	// Cargo la imagen
	if( (src = cvLoadImage (nomb_arch_entrada, CV_LOAD_IMAGE_GRAYSCALE)) == 0 )
		exit(EXIT_FAILURE);

	dst_size.width = src->width;
	dst_size.height = src->height;

	// Creo una IplImage para cada salida esperada
	if( (dst = cvCreateImage (dst_size, IPL_DEPTH_8U, 1) ) == 0 )
		exit(EXIT_FAILURE);

	// Chequeo de parametros
	if (!(min <= max && min >= 0 && max <= 255 && q >= 0 && q <= 255)) {
		imprimir_ayuda();

		cvReleaseImage(&src);
		cvReleaseImage(&dst);

		exit ( EXIT_SUCCESS );
	}

	typedef void (umbralizar_fn_t) (unsigned char*, unsigned char*, int, int, int, unsigned char, unsigned char, unsigned char);

	umbralizar_fn_t *proceso;

	if (strcmp(nomb_impl, "c") == 0) {
		proceso = umbralizar_c;
	} else {
		proceso = umbralizar_asm;
	}

	if (tiempo) {
		unsigned long long int start, end;

		MEDIR_TIEMPO_START(start);

		for(int i=0; i<cant_iteraciones; i++) {
			proceso((unsigned char*)src->imageData, (unsigned char*)dst->imageData, src->height, src->width, src->widthStep, min, max, q);
		}

		MEDIR_TIEMPO_STOP(end);

		imprimir_tiempos_ejecucion(start, end, cant_iteraciones);
	} else {
		proceso((unsigned char*)src->imageData, (unsigned char*)dst->imageData, src->height, src->width, src->widthStep, min, max, q);
	}

	// Guardo imagen y libero las imagenes
	char nomb_arch_salida[256];

	memset(nomb_arch_salida, 0, 256);

	sprintf(nomb_arch_salida, "%s.umbralizar.min-%d.max-%d.q-%d.%s.bmp", nomb_arch_entrada, min, max, q, nomb_impl);

	cvSaveImage(nomb_arch_salida, dst, NULL);

	cvReleaseImage(&src);
	cvReleaseImage(&dst);
}

void aplicar_waves (int tiempo, int cant_iteraciones, const char *nomb_impl, const char *nomb_arch_entrada, float x_scale, float y_scale, float g_scale) {
	IplImage *src = 0;
	IplImage *dst = 0;
	CvSize dst_size;

	// Cargo la imagen
	if( (src = cvLoadImage (nomb_arch_entrada, CV_LOAD_IMAGE_GRAYSCALE)) == 0 )
		exit(EXIT_FAILURE);

	dst_size.width = src->width;
	dst_size.height = src->height;

	// Creo una IplImage para cada salida esperada
	if( (dst = cvCreateImage (dst_size, IPL_DEPTH_8U, 1) ) == 0 )
		exit(EXIT_FAILURE);

	// Chequeo de parametros
	if (!(x_scale <=  32.0 && x_scale >= 0.0 &&
		  y_scale <=  32.0 && y_scale >= 0.0 &&
		  g_scale <= 255.0 && g_scale >= 0.0)) {
		imprimir_ayuda();

		cvReleaseImage(&src);
		cvReleaseImage(&dst);

		exit ( EXIT_SUCCESS );
	}

	typedef void (waves_fn_t) (unsigned char*, unsigned char*, int, int, int, float, float, float);

	waves_fn_t *proceso;

	if (strcmp(nomb_impl, "c") == 0) {
		proceso = waves_c;
	} else {
		proceso = waves_asm;
	}

	if (tiempo) {
		unsigned long long int start, end;

		MEDIR_TIEMPO_START(start);

		for(int i=0; i<cant_iteraciones; i++) {
			proceso((unsigned char*)src->imageData, (unsigned char*)dst->imageData, src->height, src->width, src->widthStep, x_scale, y_scale, g_scale);
		}

		MEDIR_TIEMPO_STOP(end);

		imprimir_tiempos_ejecucion(start, end, cant_iteraciones);
	} else {
		proceso((unsigned char*)src->imageData, (unsigned char*)dst->imageData, src->height, src->width, src->widthStep, x_scale, y_scale, g_scale);
	}

	// Guardo imagen y libero las imagenes
	char nomb_arch_salida[256];

	memset(nomb_arch_salida, 0, 256);

	sprintf(nomb_arch_salida, "%s.waves.x_scale-%3.2f.y_scale-%3.2f.g_scale-%3.2f.%s.bmp", nomb_arch_entrada, x_scale, y_scale, g_scale, nomb_impl);

	cvSaveImage(nomb_arch_salida, dst, NULL);

	cvReleaseImage(&src);
	cvReleaseImage(&dst);
}
