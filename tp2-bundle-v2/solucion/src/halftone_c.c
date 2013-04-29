#define BLACK 0
#define WHITE 255
void halftone_c (
	unsigned char *src,
	unsigned char *dst,
	int h,
	int w,
	int src_row_size,
	int dst_row_size
) {
	unsigned char (*src_matrix)[src_row_size] = (unsigned char (*)[src_row_size]) src;
	unsigned char (*dst_matrix)[dst_row_size] = (unsigned char (*)[dst_row_size]) dst;

  int i, j, sum;
  
  for (i = 0; i < h; i += 2) {
    for (j = 0; j < w; j += 2) {
      sum = (int) src_matrix[i][j] + src_matrix[i][j + 1]
          + src_matrix[i + 1][j] + src_matrix[i + 1][j + 1];
      dst_matrix[i][j] = sum < 205 ? BLACK : WHITE;
      dst_matrix[i][j + 1] = sum < 820 ? BLACK : WHITE;
      dst_matrix[i + 1][j] = sum < 615 ? BLACK : WHITE;
      dst_matrix[i + 1][j + 1] = sum < 410 ? BLACK : WHITE;
    }
  }
}
