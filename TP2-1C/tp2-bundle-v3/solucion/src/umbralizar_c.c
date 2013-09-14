#include <math.h>

void umbralizar_c (
	unsigned char *src,
	unsigned char *dst,
	int h,
	int w,
	int row_size,
	unsigned char min,
	unsigned char max,
	unsigned char q
) {
	//unsigned char (*src_matrix)[row_size] = (unsigned char (*)[row_size]) src;
	//unsigned char (*dst_matrix)[row_size] = (unsigned char (*)[row_size]) dst;
//
	int dif = row_size - w;

	int i,j;

	for (i = h; i > 0;--i)
	{
		for (j = w; j > 0;--j)
		{
			if (*src < min)
			{
				*dst = 0;
			}
			else if (*src > max)
			{
				*dst = 255;
			}else
			{
				*dst = (*src/q)*q;
			}

			src++;
			dst++;
		}

		src += dif;
		dst += dif;
	}

}
