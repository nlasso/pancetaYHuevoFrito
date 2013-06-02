int maximo_RGB(int fila, int columna, unsigned char *src, int src_row_size){
	unsigned char (*src_matrix)[src_row_size] = (unsigned char (*)[src_row_size]) src;
	int max = src_matrix[fila][columna];
	int _fila = fila - 1;
	int _columna = columna - 3;	
	//if((fila > 1 && columna > 3))
	//{
		while(_fila <= fila + 1){
			while(_columna <= columna + 3){
				if(max < src_matrix[_fila][_columna]){
					max = src_matrix[_fila][_columna];
				}
				_columna += 3;
			}
			_columna -= 9;
			_fila++;
		}
	//}
	return max;
}

float phi_R(int fila, int columna, unsigned char *src, int src_row_size, float alpha){
	int max_B = maximo_RGB(fila, columna - 2, src, src_row_size);
	int max_G = maximo_RGB(fila, columna - 1, src, src_row_size);
	int max_R = maximo_RGB(fila, columna, src, src_row_size); 
    float phi = 0;
    if((max_R >= max_G) && (max_R >= max_B)){
		phi = 1 + alpha;
    }
    else{
        phi = 1 - alpha;
    }
    return phi;
}

float phi_G(int fila, int columna, unsigned char *src, int src_row_size, float alpha){
	int max_R = maximo_RGB(fila, columna + 1, src, src_row_size);
	int max_B = maximo_RGB(fila, columna - 1, src, src_row_size);
	int max_G = maximo_RGB(fila, columna, src, src_row_size);
    float phi = 0;
    if( (max_R < max_G) && (max_G >= max_B)){
		phi = 1 + alpha;
    }
    else{
        phi = 1 - alpha;
    }
    return phi;
}

float phi_B(int fila, int columna, unsigned char *src, int src_row_size, float alpha){
	int max_R = maximo_RGB(fila, columna + 2, src, src_row_size);
	int max_G = maximo_RGB(fila, columna + 1, src, src_row_size);
	int max_B = maximo_RGB(fila, columna , src, src_row_size);
    float phi = 0;
    if((max_R < max_B) && (max_G < max_B)){        
		phi = 1 + alpha;
    }
    else{
        phi = 1 - alpha;
    }
    return phi;
}

float min_R(int fila, int columna, unsigned char *src, int src_row_size, float alpha){
	unsigned char (*src_matrix)[src_row_size] = (unsigned char (*)[src_row_size]) src;
    float res = 0;
    float phi = phi_R(fila, columna, src, src_row_size, alpha);
    int val = src_matrix[fila][columna];
    if(255 > (phi * val)){
        res = phi * val;
    }
    else{
        res = 255;
    }
    return res;
}

float min_G(int fila, int columna, unsigned char *src, int src_row_size, float alpha){
	unsigned char (*src_matrix)[src_row_size] = (unsigned char (*)[src_row_size]) src;
    float res = 0;
    float phi = phi_G(fila, columna, src, src_row_size, alpha);
    int val = src_matrix[fila][columna];
    if(255 > (phi * val)){
        res = phi * val;
    }
    else{
        res = 255;
    }
    return res;
}

float min_B(int fila, int columna, unsigned char *src, int src_row_size, float alpha){
	unsigned char (*src_matrix)[src_row_size] = (unsigned char (*)[src_row_size]) src;
    float res = 0;
    float phi = phi_B(fila, columna, src, src_row_size, alpha);
    int val = src_matrix[fila][columna];
    if(255 > (phi * val)){
        res = phi * val;
    }
    else{
        res = 255;
    }
    return res;
}



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
    int fila = 1;
    int columna = 3;
	while(fila < h - 1){
        while(columna < w*3 - 3){
			
            dst_matrix[fila][columna] = min_B(fila, columna, src, src_row_size, alpha);
            dst_matrix[fila][columna + 1] = min_G(fila, columna + 1, src, src_row_size, alpha);
            dst_matrix[fila][columna + 2] = min_R(fila, columna + 2, src, src_row_size, alpha);
            columna += 3;
        }
        fila++;
        columna = 3;
	}

}
