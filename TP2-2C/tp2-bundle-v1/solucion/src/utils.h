#ifndef __UTILS__H__
#define __UTILS__H__

void copiar_bordes (
	unsigned char *src,
	unsigned char *dst,
	int m,
	int n,
	int row_size
);

void voltear_horizontal (
	unsigned char *src,
	unsigned char *dst,
	int m,
	int n,
	int row_size
);

void pintar_bordes_negro(unsigned char *frame, int m, int n);

#endif /* !__UTILS__H__ */
