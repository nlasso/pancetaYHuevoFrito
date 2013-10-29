/*
    ./tp2 fcolor -f ../data/base/frames/ -i c ../data/base/city.avi 29 97 169 70
    ../tools/tp2diff ../data/base/frames/city.avi.fcolor.1.bmp /home/agustin/Descargas/test-fcolor-city/city.avi.fcolor.1.bmp 6
*/
#include <math.h>
#include <stdio.h>

int distancia (unsigned char rc, unsigned char gc, unsigned char bc, int r, int g, int b){
    return sqrt(pow((r - rc),2) + pow((g - gc),2) + pow((b - bc),2));
}


void color_filter_c(unsigned char *src, 
                    unsigned char *dst, 
                    unsigned char rc, 
                    unsigned char gc, 
                    unsigned char bc, 
                    int threshold, 
                    int width, 
                    int height)
{
    unsigned char (*src_matrix)[width * 3] = (unsigned char (*)[width * 3]) src;
    unsigned char (*dst_matrix)[width * 3] = (unsigned char (*)[width * 3]) dst;
    int _row = 0;
    int _column = 0;
    int newWidth = width * 3 - 3;
    int b, g, r, dist, rgb;
    while(_row <= height - 1){
        for (_column = 0; _column < newWidth; _column+=18) {

            b = src_matrix[_row][_column];
            g = src_matrix[_row][_column + 1];
            r = src_matrix[_row][_column + 2];
            dist = distancia(rc,gc,bc, r, g, b);
            if(dist > threshold){
                rgb = (r + g + b)/3;
                dst_matrix[_row][_column] = rgb;
                dst_matrix[_row][_column + 1] = rgb;
                dst_matrix[_row][_column + 2] = rgb;
            }else{
                dst_matrix[_row][_column] = b;
                dst_matrix[_row][_column + 1] = g;
                dst_matrix[_row][_column + 2] = r;
            }


            b = src_matrix[_row][_column + 3];
            g = src_matrix[_row][_column + 4];
            r = src_matrix[_row][_column + 5];
            dist = distancia(rc,gc,bc, r, g, b);
            if(dist > threshold){
                rgb = (r + g + b)/3;
                dst_matrix[_row][_column + 3] = rgb;
                dst_matrix[_row][_column + 4] = rgb;
                dst_matrix[_row][_column + 5] = rgb;
            }else{
                dst_matrix[_row][_column + 3] = src_matrix[_row][_column + 3];
                dst_matrix[_row][_column + 4] = src_matrix[_row][_column + 4];
                dst_matrix[_row][_column + 5] = src_matrix[_row][_column + 5];
            }


            b = src_matrix[_row][_column + 6];
            g = src_matrix[_row][_column + 7];
            r = src_matrix[_row][_column + 8];
            dist = distancia(rc,gc,bc, r, g, b);
            if(dist > threshold){
                rgb = (r + g + b)/3;
                dst_matrix[_row][_column + 6] = rgb;
                dst_matrix[_row][_column + 7] = rgb;
                dst_matrix[_row][_column + 8] = rgb;
            }else{
                dst_matrix[_row][_column + 6] = src_matrix[_row][_column + 6];
                dst_matrix[_row][_column + 7] = src_matrix[_row][_column + 7];
                dst_matrix[_row][_column + 8] = src_matrix[_row][_column + 8];
            }


            b = src_matrix[_row][_column + 9];
            g = src_matrix[_row][_column + 10];
            r = src_matrix[_row][_column + 11];
            dist = distancia(rc,gc,bc, r, g, b);
            if(dist > threshold){
                rgb = (r + g + b)/3;
                dst_matrix[_row][_column + 9] = rgb;
                dst_matrix[_row][_column + 10] = rgb;
                dst_matrix[_row][_column + 11] = rgb;
            }else{
                dst_matrix[_row][_column + 9] = src_matrix[_row][_column + 9];
                dst_matrix[_row][_column + 10] = src_matrix[_row][_column + 10];
                dst_matrix[_row][_column + 11] = src_matrix[_row][_column + 11];
            }


            b = src_matrix[_row][_column + 12];
            g = src_matrix[_row][_column + 13];
            r = src_matrix[_row][_column + 14];
            dist = distancia(rc,gc,bc, r, g, b);
            if(dist > threshold){
                rgb = (r + g + b)/3;
                dst_matrix[_row][_column + 12] = rgb;
                dst_matrix[_row][_column + 13] = rgb;
                dst_matrix[_row][_column + 14] = rgb;
            }else{
                dst_matrix[_row][_column + 12] = src_matrix[_row][_column + 12];
                dst_matrix[_row][_column + 13] = src_matrix[_row][_column + 13];
                dst_matrix[_row][_column + 14] = src_matrix[_row][_column + 14];
            }


            b = src_matrix[_row][_column + 15];
            g = src_matrix[_row][_column + 16];
            r = src_matrix[_row][_column + 17];
            dist = distancia(rc,gc,bc, r, g, b);
            if(dist > threshold){
                rgb = (r + g + b)/3;
                dst_matrix[_row][_column + 15] = rgb;
                dst_matrix[_row][_column + 16] = rgb;
                dst_matrix[_row][_column + 17] = rgb;
            }else{
                dst_matrix[_row][_column + 15] = src_matrix[_row][_column + 15];
                dst_matrix[_row][_column + 16] = src_matrix[_row][_column + 16];
                dst_matrix[_row][_column + 17] = src_matrix[_row][_column + 17];
            }
        }
        _row++;
    }
}
