#ifndef __FILTROS__H__
#define __FILTROS__H__

/* Versiones en C */
void color_filter_c(unsigned char *src,
                    unsigned char *dst,
                    unsigned char rc,
                    unsigned char gc,
                    unsigned char bc,
                    int threshold,
                    int width,
                    int height);


void miniature_c(unsigned char *src,
                unsigned char *dst,
                int width,
                int height,
                float topPlane,
                float bottomPlane,
                int iters);

void decode_c(unsigned char *src,
              unsigned char *code,
              int size,
              int width,
              int height);
              
void color_filter_asm(unsigned char *src,
                    unsigned char *dst,
                    unsigned char rc,
                    unsigned char gc,
                    unsigned char bc,
                    int threshold,
                    int width,
                    int height);

void miniature_asm(unsigned char *src,
                unsigned char *dst,
                int width,
                int height,
                float topPlane,
                float bottomPlane,
                int iters);

void decode_asm(unsigned char *src,
              unsigned char *code,
              int size,
              int width,
              int height);
#endif /* !__FILTROS__H__ */
