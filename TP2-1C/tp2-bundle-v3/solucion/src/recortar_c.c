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
  int i, j;
  
  for (i = 0; i < tam; i++) {
    for (j = 0; j < tam; j++) {
      dst_matrix[i][j] = src_matrix[h - tam + i][w - tam + j];
      dst_matrix[i + tam][j + tam] = src_matrix[i][j];
      dst_matrix[i + tam][j] = src_matrix[i][w - tam + j];
      dst_matrix[i][j + tam] = src_matrix[h - tam + i][j];
    }
  }
}
