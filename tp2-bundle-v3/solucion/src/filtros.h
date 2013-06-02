#ifndef __FILTROS__H__
#define __FILTROS__H__

/* Versiones en C */
void recortar_c (
	unsigned char *src,
	unsigned char *dst,
	int m,
	int n,
	int src_row_size,
	int dst_row_size,
	int tam
);

void colorizar_c (
	unsigned char *src,
	unsigned char *dst,
	int m,
	int n,
	int src_row_size,
	int dst_row_size,
	float alpha
);

void halftone_c (
	unsigned char *src,
	unsigned char *dst,
	int m,
	int n,
	int src_row_size,
	int dst_row_size
);

void rotar_c (
	unsigned char *src,
	unsigned char *dst,
	int m,
	int n,
	int src_row_size,
	int dst_row_size
);

void umbralizar_c (
	unsigned char *src,
	unsigned char *dst,
	int m,
	int n,
	int row_size,
	unsigned char min,
	unsigned char max,
	unsigned char q
);

void waves_c (
	unsigned char *src,
	unsigned char *dst,
	int m,
	int n,
	int row_size,
	float x_scale,
	float y_scale,
	float g_scale
);

/* Versiones en ASM */
void recortar_asm (
	unsigned char *src,
	unsigned char *dst,
	int m,
	int n,
	int src_row_size,
	int dst_row_size,
	int tam
);

void colorizar_asm (
	unsigned char *src,
	unsigned char *dst,
	int m,
	int n,
	int src_row_size,
	int dst_row_size,
	float alpha
);

void halftone_asm (
	unsigned char *src,
	unsigned char *dst,
	int m,
	int n,
	int src_row_size,
	int dst_row_size
);

void rotar_asm (
	unsigned char *src,
	unsigned char *dst,
	int m,
	int n,
	int src_row_size,
	int dst_row_size
);

void umbralizar_asm (
	unsigned char *src,
	unsigned char *dst,
	int m,
	int n,
	int row_size,
	unsigned char min,
	unsigned char max,
	unsigned char q
);

void waves_asm (
	unsigned char *src,
	unsigned char *dst,
	int m,
	int n,
	int row_size,
	float x_scale,
	float y_scale,
	float g_scale
);

#endif /* !__FILTROS__H__ */
