void copiar_bordes (
	unsigned char *src,
	unsigned char *dst,
	int m,
	int n,
	int row_size
) {
	for(int j = 0; j<n; j++) {
		// superior
		dst[0*row_size+j] = src[0*row_size+j];
		// inferior
		dst[(m-1)*row_size+j] = src[(m-1)*row_size+j];
	}

	for(int i = 0; i<m; i++) {
		// izquierdo
		dst[i*row_size+0] = src[i*row_size+0];
		// derecho
		dst[i*row_size+(n-1)] = src[i*row_size+(n-1)];
	}
}

void copiar_bordes_color (
	unsigned char *src,
	unsigned char *dst,
	int m,
	int n,
	int row_size
) {
	unsigned char (*src_matrix)[row_size] = (unsigned char (*)[row_size]) src;
	unsigned char (*dst_matrix)[row_size] = (unsigned char (*)[row_size]) dst;

	for(int j = 0, k = 0; j<n; j++, k+=3) {
		// superior
		dst_matrix[0][k+0] = src_matrix[0][k+0];
		dst_matrix[0][k+1] = src_matrix[0][k+1];
		dst_matrix[0][k+2] = src_matrix[0][k+2];

		// inferior
		dst_matrix[m-1][k+0] = src_matrix[m-1][k+0];
		dst_matrix[m-1][k+1] = src_matrix[m-1][k+1];
		dst_matrix[m-1][k+2] = src_matrix[m-1][k+2];
	}

	for(int i = 0; i<m; i++) {
		// izquierdo
		dst_matrix[i][0+0] = src_matrix[i][0+0];
		dst_matrix[i][0+1] = src_matrix[i][0+1];
		dst_matrix[i][0+2] = src_matrix[i][0+2];

		// derecho
		dst_matrix[i][(n*3-3)+0] = src_matrix[i][(n*3-3)+0];
		dst_matrix[i][(n*3-3)+1] = src_matrix[i][(n*3-3)+1];
		dst_matrix[i][(n*3-3)+2] = src_matrix[i][(n*3-3)+2];
	}
}

void voltear_horizontal (
	unsigned char *src,
	unsigned char *dst,
	int m,
	int n,
	int row_size
) {
	unsigned char (*src_matrix)[row_size] = (unsigned char (*)[row_size]) src;
	unsigned char (*dst_matrix)[row_size] = (unsigned char (*)[row_size]) dst;

	for (int i = 0; i<m; i+=1) {
		for (int j = 0; j<n; j+=1) {
			dst_matrix[i][n-j-1] = src_matrix[i][j];
		}
	}
}