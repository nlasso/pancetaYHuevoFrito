#include <math.h>
#include <stdio.h>

int distancia (int rc, int gc, int bc, int r, int g, int b, int threshold){
	int res = sqrt(pow((r - rc),2) + pow((g - gc),2) + pow((b - bc),2));
	if(res > threshold){
		return 1;
	}
	return 0;
}


void color_filter_c(unsigned char *src, 
					unsigned char *dst, 
					int rc, 
					int gc, 
					int bc, 
					int threshold, 
					int width, 
					int height)
{
	unsigned char (*src_matrix)[width * 3] = (unsigned char (*)[width * 3]) src;
	unsigned char (*dst_matrix)[width * 3] = (unsigned char (*)[width * 3]) dst;
	int _row = 0;
	int _column = 0;
	while(_row <= height ){
		while(_column <= width * 3){
			int b = src_matrix[_row][_column];
			int g = src_matrix[_row][_column + 1];
			int r = src_matrix[_row][_column + 2];
			int dist = distancia(rc,gc,bc, r, g, b, threshold);
			if(dist == 1){
				int rgb = (r + g + b)/3;
				dst_matrix[_row][_column] = rgb;
				dst_matrix[_row][_column + 1] = rgb;
				dst_matrix[_row][_column + 2] = rgb;
			}else{
				dst_matrix[_row][_column] = src_matrix[_row][_column];
				dst_matrix[_row][_column + 1] = src_matrix[_row][_column + 1];
				dst_matrix[_row][_column + 2] = src_matrix[_row][_column + 2];
			}
			_column = _column + 3;			
		}
		_column = 3;
		_row++;
	}
}
