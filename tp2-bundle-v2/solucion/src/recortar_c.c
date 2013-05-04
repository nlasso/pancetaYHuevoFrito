void recortar_c (
	unsigned char *src,
	unsigned char *dst,
	int h,
	int w,
	int src_row_size,
	int dst_row_size,
	int tam
) {
	unsigned char (*src_matrix)[src_row_size] = (unsigned char (*)[src_row_size]) src;
	unsigned char (*dst_matrix)[dst_row_size] = (unsigned char (*)[dst_row_size]) dst;

    int fila = 0;
    int columna = 0;
	while(fila <= tam){
        while(columna <= tam){
            dst_matrix[fila][columna] = src_matrix[h - tam + fila][w - tam + columna];
            dst_matrix[fila][2*tam - columna] = src_matrix[h - tam + fila][tam - columna];
            dst_matrix[2*tam - fila][columna] = src_matrix[tam - fila][w - tam + columna];
            dst_matrix[2*tam - fila][2*tam - columna] = src_matrix[tam - fila][tam - columna];
            columna++;
        }
        fila++;
	}

}
