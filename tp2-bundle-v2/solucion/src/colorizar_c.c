void colorizar_c (
	unsigned char *src,
	unsigned char *dst,
	int h,
	int w,
	int src_row_size,
	int dst_row_size,
	float alpha
) {
	unsigned char (*src_matrix)[src_row_size] = (unsigned char (*)[src_row_size]) src;
	unsigned char (*dst_matrix)[dst_row_size] = (unsigned char (*)[dst_row_size]) dst;
    int fila = 3;
    int columna = 3;
	while(fila < h){
        while(columna < w*3 - 3){
            (*dst_matrix)[fila][columna] = min_B(fila, columna, *src_matrix, alpha);
            (*dst_matrix)[fila][columna + 1] = min_G(fila, columna + 1, *src_matrix, alpha);
            (*dst_matrix)[fila][columna + 2] = min_R(fila, columna + 2, *src_matrix, alpha);
            columna += 3;
        }
        fila++;
	}

}

float min_R(int fila, int columna, unsigned char *src, float alpha){
    float res = 0;
    int phi = phi_R(fila, columna, *src, alpha);
    int val = *src[fila][columna];
    if(255 > (phi * val)){
        res = phi * val;
    }
    else{
        res = 255;
    }
}

float min_G(int fila, int columna, unsigned char *src, float alpha){
    float res = 0;
    int phi = phi_G(fila, columna, *src, alpha);
    int val = *src[fila][columna];
    if(255 > (phi * val)){
        res = phi * val;
    }
    else{
        res = 255;
    }
}

float min_B(int fila, int columna, unsigned char *src, float alpha){
    float res = 0;
    int phi = phi_B(fila, columna, *src, alpha);
    int val = *src[fila][columna];
    if(255 > (phi * val)){
        res = phi * val;
    }
    else{
        res = 255;
    }
}

int maximo_RGB(int fila, int columna, unsigned char *src){
    int _fila = fila - 1;
    int _columna = columna - 3;
    int max = (*src)[_fila][_columna];
    while(_fila <= fila + 1){
        while(_columna <= columna + 3){
            if(max < (*src)[_fila][_columna]){
                max = (*src)[_fila][_columna];
            }
            _columna += 3;
        }
        _columna -= 9;
        _fila++;
    }
    return max;
}

float phi_R(int fila, int columna, unsigned char *src, float alpha){
    int phi = 0;
    if(maximo_RGB(fila, columna, *src) >= maximo_RGB(fila, columna - 1, *src)){
        if(maximo_RGB(fila, columna, *src) >= maximo_RGB(fila, columna - 2, *src)){
            phi = 1 + alpha;
        }
    }
    else{
        phi = 1 - alpha;
    }
    return phi;
}

float phi_G(int fila, int columna, unsigned char *src, float alpha){
    int phi = 0;
    if(maximo_RGB(fila, columna + 1, *src) < maximo_RGB(fila, columna, *src)){
        if(maximo_RGB(fila, columna, *src) >= maximo_RGB(fila, columna - 1, *src)){
            phi = 1 + alpha;
        }
    }
    else{
        phi = 1 - alpha;
    }
    return phi;
}

float phi_B(int fila, int columna, unsigned char *src, float alpha){
    int phi = 0;
    if(maximo_RGB(fila, columna + 2, *src) < maximo_RGB(fila, columna + 1, *src)){
        if(maximo_RGB(fila, columna + 1, *src) < maximo_RGB(fila, columna , *src)){
            phi = 1 + alpha;
        }
    }
    else{
        phi = 1 - alpha;
    }
    return phi;
}














