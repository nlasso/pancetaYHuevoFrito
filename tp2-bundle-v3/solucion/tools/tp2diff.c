#include <getopt.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>

const char *nombre_programa;

void imprimir_ayuda();

int main(int argc, char **argv) {
	int siguiente_opcion;

	// Opciones
	const char* const op_cortas = "hq";

	const struct option op_largas[] = {
		{ "help", 0, NULL, 'h' },
		{ "quite", 0, NULL, 'q' },
		{ NULL, 0, NULL, 0 }
	};

	// Flags de opciones
	int quite = 0;

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
			case 'q' : /* -q o --quite */
				quite = 1;
				break;
			case '?' : /* opcion no valida */
				imprimir_ayuda ( );
				exit ( EXIT_SUCCESS );
			default : /* opcion no valida */
				abort ( );
		}
	}

	const char *filename_a = argv[optind++];

	if (filename_a == NULL) {
		imprimir_ayuda ( );

		exit ( EXIT_SUCCESS );
	}

	const char *filename_b = argv[optind++];

	if (filename_b == NULL) {
		imprimir_ayuda ( );

		exit ( EXIT_SUCCESS );
	}

	const char *epsilon_str = argv[optind++];

	if (epsilon_str == NULL) {
		imprimir_ayuda ( );

		exit ( EXIT_SUCCESS );
	}

	int epsilon = atoi(epsilon_str);

	// Cargo las imagenes
	IplImage *src_a = 0;
	IplImage *src_b = 0;
	IplImage *dst = 0;
	int width;
	int height;

	if( (src_a = cvLoadImage (filename_a, CV_LOAD_IMAGE_GRAYSCALE)) == 0 )
	{
		printf("Error al cargar: %s\n", filename_a);

		exit(EXIT_FAILURE);
	}

	if( (src_b = cvLoadImage (filename_b, CV_LOAD_IMAGE_GRAYSCALE)) == 0 )
	{
		printf("Error al cargar: %s\n", filename_b);

		cvReleaseImage(&src_a);

		exit(EXIT_FAILURE);
	}

	// Chequear tamanos de las imagenes
	if (src_a->width != src_b->width || src_a->height != src_b->height) {
		printf("Los tamaños de las imágenes no distintos.\n");

		cvReleaseImage(&src_a);
		cvReleaseImage(&src_b);

		exit(EXIT_FAILURE);
	}

	// Creo una IplImage para cada salida esperada
	if( (dst = cvCreateImage (cvGetSize(src_a), IPL_DEPTH_8U, 1) ) == 0 ) {
		printf("Error al crear imágen de salida.\n");

		cvReleaseImage(&src_a);
		cvReleaseImage(&src_b);

		exit(EXIT_FAILURE);
	}

	// Chequeo pixel a pixel
	width = src_a->width;
	height = src_a->height;

	int row_size_a = src_a->widthStep;
	int row_size_b = src_b->widthStep;
	int row_size_dst = dst->widthStep;

	int equal = 1;

	unsigned char (*src_a_matrix)[row_size_a] = (unsigned char (*)[row_size_a]) src_a->imageData;
	unsigned char (*src_b_matrix)[row_size_b] = (unsigned char (*)[row_size_b]) src_b->imageData;
	unsigned char (*dst_matrix)[row_size_dst] = (unsigned char (*)[row_size_dst]) dst->imageData;

	int pixel_a;
	int pixel_b;

	for (int i = 0; i<height; i++) {
		for (int j = 0; j<width; j++) {
			pixel_a = (int) src_a_matrix[i][j];
			pixel_b = (int) src_b_matrix[i][j];

			if (abs(pixel_a - pixel_b) > epsilon) {
				if (!quite) {
					printf("Hay diferencias en el pixel: (%d, %d)\n", i, j);
				}

				equal = 0;

				dst_matrix[i][j] = 255;
			} else {
				dst_matrix[i][j] = 0;
			}
		}
	}

	// Guardo imagen y libero las imagenes
	char nomb_arch_salida[256];

	memset(nomb_arch_salida, 0, 256);

	sprintf(nomb_arch_salida, "diferencia.bmp");

	cvSaveImage(nomb_arch_salida, dst, NULL);

	// Libero las imagenes
	cvReleaseImage(&src_a);
	cvReleaseImage(&src_b);
	cvReleaseImage(&dst);

	return !equal;
}

void imprimir_ayuda ( ) {
	printf ( "Uso: %s opciones nombre_archivo_entrada_a nombre_archivo_entrada_b epsilon \n", nombre_programa );
	printf ( "Ejemplo de uso:                                                   \n" );
	printf ( "    %s lena_a.bmp lena_b.bmp 5                                    \n", nombre_programa );
	printf ( "                                                                  \n" );
	printf ( "    Verifica pixel a pixel que la diferencia entre lena_a.bmp y   \n" );
	printf ( "    lena_b.bmp no supere el valor 5. Genera una imágen a partir   \n" );
	printf ( "    de las diferencias, poniendo un pixel blanco donde haya       \n" );
	printf ( "    diferencias y uno negro donde no.                             \n" );
	printf ( "																	\n" );
	printf ( "    -h, --help		Imprime esta ayuda                          \n" );
	printf ( "                                                                  \n" );
	printf ( "    -q, --quite       Ejecuta en modo silencioso.		            \n" );
}