#include <math.h>
void rotar_c (
  unsigned char *src,
  unsigned char *dst,
  int h,
  int w,
  int src_row_size,
  int dst_row_size
) {
  unsigned char (*src_matrix)[src_row_size] = (unsigned char (*)[src_row_size]) src;
  unsigned char (*dst_matrix)[dst_row_size] = (unsigned char (*)[dst_row_size]) dst;
  int i, j,
    cx = w / 2,
    cy = h / 2;
  float u, v;

  for (i = 0; i < h; i++) {
    for (j = 0; j < w; j++) {
	    u = cx + (j - cx) * sqrt(2) / 2 - (i - cy) * sqrt(2) / 2;
	    v = cy + (j - cx) * sqrt(2) / 2 + (i - cy) * sqrt(2) / 2;
      
      if (0 <= (int) u && u < w && 0 <= (int) v && v < h) {
        dst_matrix[i][j] = src_matrix[(int) v][(int) u];
      } else {
        dst_matrix[i][j] = 0;
      }
    }
  }
}
