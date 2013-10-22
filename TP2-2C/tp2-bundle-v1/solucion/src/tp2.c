#include <getopt.h>
#include <highgui.h>
#include <cv.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "filtros.h"
#include "tiempo.h"
#include "utils.h"

typedef struct CvVideoWriter CvVideoWriter;

const char* nombre_programa;

void aplicar_filtro_color (const char *implementacion,
                           const char *archivo_entrada,
                           bool verbose, bool frames,
                           const char *carpeta_frames,
                           int rc, int gc, int bc,
                           int threshold);

void aplicar_miniature(const char *implementacion,
                       const char *archivo_entrada,
                       bool verbose, bool frames,
                       const char *carpeta_frames,
                       float topPlane, float bottomPlane,
                       int iters);

void aplicar_decode   (const char *implementacion,
                       const char *archivo_entrada,                       
                       bool verbose);

void aplicar_original (const char *archivo_entrada,
                       bool verbose, bool frames,
                       const char *carpeta_frames);

void imprimir_ayuda ( );
void imprimir_tiempos_ejecucion(unsigned long long int start, unsigned long long int end, int cant_iteraciones);
static const char *basename(const char *path);

typedef enum {FCOLOR, MINIATURE, DECODE, ENCODE} filter_type;

int main( int argc, char** argv ) {
	int siguiente_opcion;

	// Opciones
	const char* const op_cortas = "hi:vt:f:";

	const struct option op_largas[] = {
		{ "help", 0, NULL, 'h' },
		{ "implementacion", 1, NULL, 'i' },
		{ "verbose", 0, NULL, 'v' },
		{ "tiempo", 1, NULL, 't' },
		{ "frames", 1, NULL, 'f' },
		{ NULL, 0, NULL, 0 }
	};

	// Parametros
	const char *nombre_implementacion = NULL;
	int cant_iteraciones = 0;

	// Flags de opciones
	bool verbose = false;
	int tiempo = 0;
	bool frames = false;
	const char *carpeta_frames = NULL;

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
				verbose = true;
				break;
			case 'f' : /* -f o --frames */
			    frames = 1;
			    carpeta_frames = optarg;
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
		(strcmp(nomb_proceso, "miniature") != 0			&&
		strcmp(nomb_proceso, "decode") != 0			&&        
		strcmp(nomb_proceso, "fcolor") != 0			&&
		strcmp(nomb_proceso, "original") != 0)) {
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

	// Verifico carpeta de frames
	if ( frames ) {
        struct stat s;
        int err = stat(carpeta_frames, &s);

        if (err == -1 || !S_ISDIR(s.st_mode)) {
            fprintf(stderr, "Carpeta de Frames invalida: %s.\n", carpeta_frames);
            exit (EXIT_FAILURE) ;
        }
	}

	// Imprimo info
	printf ( "Procesando...\n");
	printf ( "  Filtro             : %s\n", nomb_proceso);
	printf ( "  Implementación     : %s\n", nombre_implementacion);
	printf ( "  Archivo de entrada : %s\n", nomb_arch_entrada);

	// Procesar imagen
    if (strcmp(nomb_proceso, "fcolor") == 0) {
        int threshold, gc, rc, bc;
        threshold = atoi(argv[argc - 1]);
        bc = atoi(argv[argc - 2]);
        gc = atoi(argv[argc - 3]);
        rc = atoi(argv[argc - 4]);
		aplicar_filtro_color(nombre_implementacion, nomb_arch_entrada,
		                    verbose, frames, carpeta_frames,
		                    rc, gc, bc, threshold);
	} else if (strcmp(nomb_proceso, "decode") == 0) {

        aplicar_decode(nombre_implementacion, nomb_arch_entrada,
                        verbose);
       

	} else if (strcmp(nomb_proceso, "miniature") == 0) {
	    int iters;
	    float bottomPlane, topPlane;
        iters = atoi(argv[argc - 1]);
        bottomPlane = strtof(argv[argc - 2], NULL);
        topPlane = strtof(argv[argc - 3], NULL);

		aplicar_miniature(nombre_implementacion, nomb_arch_entrada,
                    verbose, frames, carpeta_frames,
                    topPlane, bottomPlane, iters);
	} else if (strcmp(nomb_proceso, "original") == 0) {
        aplicar_original(nomb_arch_entrada, verbose, frames,
                         carpeta_frames);
    }



	return 0;
}

void imprimir_ayuda ( ) {
	printf ( "Uso: %s opciones filtro nombre_archivo_entrada parametros_filtro\n",
	         nombre_programa );
	printf ( "    Los filtros que se pueden aplicar son \n");
	printf ( "       * fcolor\n" );
	printf ( "           Parámetros     : \n"
	         "                         r intensidad del color Rojo [0, 255]\n"
	         "                         g intensidad del color Verde [0, 255]\n"
	         "                         b intensidad del color Azul [0, 255]\n"
	         "                         treshold distancia tolerable hacia ese color\n");
	printf ( "           Ejemplo de uso : \n"
	         "                         fcolor -i c video.avi 0 230 0 4000\n" );
	printf ( "       * miniature\n" );
	printf ( "           Parámetros     : \n"
	         "                         topPlane parte superior de la pantalla a blurear [0, 1)\n"
	         "                         bottomPlane parte inferior de la pantalla a blurear [0, 1)\n"
	         "                         iteraciones cantidad de Iteraciones\n");
	printf ( "           Ejemplo de uso : \n"
	         "                         miniature -i c video.avi 0.3 0.8 3\n" );
	printf ( "       * decode\n" );
	printf ( "           Parámetros     : \n" );
	printf ( "           Ejemplo de uso : \n"
             "                         decode -i c video_con_mensaje.avi\n" );
	printf ( "       * original\n" );
	printf ( "           Parámetros     : \n" );
	printf ( "           Ejemplo de uso : \n"
             "                         original -i c video.avi\n" );
	printf ( "\n" );
	printf ( "    -h, --help: \n");
	printf ( "                Imprime esta ayuda\n" );
    printf ( "\n" );
	printf ( "    -i, --implementacion NOMBRE_MODO\n");
	printf ( "                                     "
	         "Implementación sobre la que se ejecutará el filtro\n"
	         "                                     "
	         "seleccionado. Los implementaciones disponibles\n"
	         "                                     "
	         "son: c, asm\n");
	printf ( "\n" );
	printf ( "    -t, --tiempo CANT_ITERACIONES\n");
	printf ( "                                   "
	         "Mide el tiempo que tarda en ejecutar el filtro sobre la\n"
	         "                                   "
	         "imagen de entrada una cantidad de veces igual a\n"
	         "                                   "
	         "CANT_ITERACIONES\n");
	printf ( "    -f, --frames CARPETA\n");
	printf ( "                          "
	         "Genera frames independientes en CARPETA en vez de armar\n"
	         "                          "
	         "un archivo de video. Es utilizado para testing.");
	printf ( "\n" );
	printf ( "    -v, --verbose\n");
	printf ( "                   "
	         "Imprime información adicional\n" );
	printf ( "\n" );
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

static struct CvVideoWriter *abrir_writer(const char *archivo_salida, int fps, CvSize size) {
    struct CvVideoWriter *dstVid = NULL;
    dstVid = cvCreateVideoWriter(archivo_salida, CV_FOURCC('M','J','P','G'), fps, size, 1);
    if(dstVid == NULL) {
        fprintf(stderr, "Invalid dstVid\n");
        exit(EXIT_FAILURE);
    }

    return dstVid;
}

static CvCapture *abrir_video(const char *archivo_entrada) {
    CvCapture *srcVid = NULL;
    srcVid = cvCaptureFromFile(archivo_entrada);
    if( srcVid == NULL) {
        /* Esto no está documentado. No debería pasar nunca. */
        fprintf(stderr, "Invalid srcVid\n%s \n", archivo_entrada);
        exit(EXIT_FAILURE);
    }

    return srcVid;
}

static const char *basename(const char *path) {
    const char *res = strrchr(path, '/');
    if (res != NULL) {
        return res + 1;
    }
    return path;
}

void aplicar_miniature(const char *implementacion,
                       const char *archivo_entrada,
                       bool verbose, bool frames,
                       const char *carpeta_frames,
                       float topPlane, float bottomPlane,
                       int iters) {

    static size_t bufsize = 1024;
    char namebuff[bufsize + 1];
    const char *filename = basename(archivo_entrada);

    CvCapture *srcVid = abrir_video(archivo_entrada);
	CvSize dst_size;

    dst_size.width = cvGetCaptureProperty(srcVid,CV_CAP_PROP_FRAME_WIDTH);
    dst_size.height = cvGetCaptureProperty(srcVid,CV_CAP_PROP_FRAME_HEIGHT);
	double fps = cvGetCaptureProperty(srcVid,CV_CAP_PROP_FPS);

    typedef struct CvVideoWriter CvVideoWriter;
    CvVideoWriter* dstVid = NULL;

    if (!frames) {
        snprintf(namebuff, bufsize, "%s.miniature.avi", archivo_entrada);
        dstVid = abrir_writer(namebuff, fps, dst_size);
    }

    int nchannels = 3;
    /* Armo la imagen destino. */
	IplImage *dst = NULL;
    dst = cvCreateImage (dst_size, IPL_DEPTH_8U, nchannels);
    if (dst == NULL) {
        fprintf(stderr, "Error armando la imagen destino\n");
        exit(EXIT_FAILURE);
    }

    if (verbose) {
        cvNamedWindow("procesanding", CV_WINDOW_AUTOSIZE);
    }

    unsigned int framenum = 0;
    while(1) {
        /* Capturo un frame */
        IplImage *frame = cvQueryFrame(srcVid);
        framenum ++;

        if (frame == NULL) {
            break;
        }

        /* Aplico el filtro */
        typedef void (miniature_fn_t) (unsigned char*, unsigned char*,
                                    int, int, float, float, int);

        miniature_fn_t *proceso;

        if (strcmp(implementacion, "c") == 0) {
            proceso = miniature_c;
        } else {
            proceso = miniature_asm;
        }

        proceso((unsigned char *) frame->imageData,
                (unsigned char *) dst->imageData,
                frame->width, frame->height,
                topPlane, bottomPlane, iters);

        pintar_bordes_negro((unsigned char *) dst->imageData, frame->height, frame->width);

        if (frames) {
            /* Escribo el frame */
            snprintf(namebuff, bufsize, "%s/%s.miniature.%d.bmp",
                                    carpeta_frames, filename, framenum);

            cvSaveImage(namebuff, dst, NULL);
        } else {
            cvWriteFrame(dstVid, dst);
        }
        if (verbose) {
            cvShowImage("procesanding", dst);
            cvWaitKey(1);
        }
    }

    cvReleaseImage(&dst);
    if (verbose) {
        cvDestroyWindow("procesanding");
    }

    if (!frames) {
        cvReleaseVideoWriter(&dstVid);
    }
    cvReleaseCapture(&srcVid);

}

void aplicar_filtro_color (const char *implementacion,
                           const char *archivo_entrada,
                           bool verbose, bool frames,
                           const char *carpeta_frames,
                           int rc, int gc, int bc,
                           int threshold) {
    static size_t bufsize = 1024;
    char namebuff[bufsize + 1];

    const char *filename = basename(archivo_entrada);
    CvCapture *srcVid = abrir_video(archivo_entrada);
	CvSize dst_size;

    dst_size.width = cvGetCaptureProperty(srcVid,CV_CAP_PROP_FRAME_WIDTH);
    dst_size.height = cvGetCaptureProperty(srcVid,CV_CAP_PROP_FRAME_HEIGHT);
	double fps = cvGetCaptureProperty(srcVid,CV_CAP_PROP_FPS);
    int nchannels = 3;

    typedef struct CvVideoWriter CvVideoWriter;
    CvVideoWriter* dstVid = NULL;

    if (!frames) {
        snprintf(namebuff, bufsize, "%s.fcolor.avi", archivo_entrada);
        dstVid = abrir_writer(namebuff, fps, dst_size);
    }
    /* Armo la imagen destino. */
	IplImage *dst = NULL;
    dst = cvCreateImage (dst_size, IPL_DEPTH_8U, nchannels);
    if (dst == NULL) {
        fprintf(stderr, "Error armando la imagen destino\n");
        exit(EXIT_FAILURE);
    }

    if (verbose) {
        cvNamedWindow("procesanding", CV_WINDOW_AUTOSIZE);
    }
    unsigned int framenum = 0;
    unsigned long long int startColor;
    unsigned long long int endColor;
    MEDIR_TIEMPO_START(startColor)
    while(1) {
        /* Capturo un frame */
        IplImage *frame = cvQueryFrame(srcVid);

        framenum ++;

        if (frame == NULL) {
            break;
        }

        /* Aplico el filtro */
        typedef void (fcolor_fn_t) (unsigned char*, unsigned char*,
                                    unsigned char, unsigned char,
                                    unsigned char, int, int, int);

        fcolor_fn_t *proceso;

        if (strcmp(implementacion, "c") == 0) {
            proceso = color_filter_c;
        } else {
            proceso = color_filter_asm;
        }


        
        proceso((unsigned char *) frame->imageData,
                (unsigned char *)  dst->imageData, rc, gc, bc, threshold,
                frame->height, frame->width);
        



        if (frames) {
            /* Escribo el frame */
            snprintf(namebuff, bufsize, "%s/%s.fcolor.%d.bmp",
                                    carpeta_frames, filename, framenum);
            cvSaveImage(namebuff, dst, NULL);
        } else {
            cvWriteFrame(dstVid, dst);
        }
        if (verbose) {
            cvShowImage("procesanding", dst);
            cvWaitKey(1);
        }
    }
    MEDIR_TIEMPO_STOP(endColor);
    imprimir_tiempos_ejecucion(startColor, endColor, 15000);

    if (verbose) {
        cvDestroyWindow("procesanding");
    }

    cvReleaseImage(&dst);
    if (!frames) {
        cvReleaseVideoWriter(&dstVid);
    }
    cvReleaseCapture(&srcVid);
}


void aplicar_decode   (const char *implementacion,
                       const char *archivo_entrada,                       
                       bool verbose)
{
    const int long_texto=10000;
    unsigned char mensaje_salida[long_texto];
    
    IplImage *src = 0;
	IplImage *dst = 0;

	// Cargo la imagen
	if( (src = cvLoadImage (archivo_entrada, CV_LOAD_IMAGE_COLOR)) == 0 )
		exit(EXIT_FAILURE);

	// Creo una IplImage para cada salida esperada
	if( (dst = cvCreateImage (cvGetSize (src), IPL_DEPTH_8U, 1) ) == 0 )
		exit(EXIT_FAILURE);

	typedef void (decode_t) (unsigned char*, unsigned char*, int, int, int);

	decode_t *proceso;

	if (strcmp(implementacion, "c") == 0) {
		proceso = decode_c;
	} else {
		proceso = decode_asm;
	}

    unsigned long long int start, end;
    MEDIR_TIEMPO_START(start);

    proceso((unsigned char*)src->imageData, (unsigned char*)mensaje_salida, long_texto, src->height, src->width);

    MEDIR_TIEMPO_STOP(end);
    imprimir_tiempos_ejecucion(start, end, 100);

	// Guardo imagen y libero las imagenes
    char filename[255];
        
    sprintf(filename,"%s.mensaje.%s.txt",archivo_entrada,implementacion);
    FILE* decoded_txt = fopen(filename,"w");
    fprintf(decoded_txt,"%s",mensaje_salida);
    fclose(decoded_txt);
	printf ( "  Salida en          : %s\n", filename);
	cvReleaseImage(&src);
	cvReleaseImage(&dst);
}

void aplicar_original     (const char *archivo_entrada,
                           bool verbose, bool frames,
                           const char *carpeta_frames){
    static size_t bufsize = 1024;
    char namebuff[bufsize + 1];

    const char *filename = basename(archivo_entrada);
    CvCapture *srcVid = abrir_video(archivo_entrada);
	CvSize dst_size;

    dst_size.width = cvGetCaptureProperty(srcVid,CV_CAP_PROP_FRAME_WIDTH);
    dst_size.height = cvGetCaptureProperty(srcVid,CV_CAP_PROP_FRAME_HEIGHT);
	double fps = cvGetCaptureProperty(srcVid,CV_CAP_PROP_FPS);

    typedef struct CvVideoWriter CvVideoWriter;
    snprintf(namebuff, bufsize, "%s.original.avi", archivo_entrada);
    printf("\n\n%s\n\n",namebuff);
    CvVideoWriter* dstVid = abrir_writer(namebuff, fps, dst_size);

    if (verbose) {
        cvNamedWindow("procesanding", CV_WINDOW_AUTOSIZE);
    }
    unsigned int framenum = 0;
    while(1) {
        /* Capturo un frame */
        IplImage *frame = cvQueryFrame(srcVid);

        framenum ++;
        

        if(framenum==100)break;

        cvWriteFrame(dstVid, frame);
        if (frames) {
            /* Escribo el frame */
            snprintf(namebuff, bufsize, "%s/%s.original.%d.bmp",
                                    carpeta_frames, filename, framenum);
            cvSaveImage(namebuff, frame, NULL);
        }
        if (verbose) {
            cvShowImage("procesanding", frame);
            cvWaitKey(1);
        }
    }

    if (verbose) {
        cvDestroyWindow("procesanding");
    }

    cvReleaseVideoWriter(&dstVid);
    cvReleaseCapture(&srcVid);
}
