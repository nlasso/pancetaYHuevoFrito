/*
    ./tp2 -t 1000 -f ../data/base/frames/ -i c miniature ../data/base/city.avi 0.3 0.8 5
    ./tp2diff ../data/base/frames/city.avi.miniature.1.bmp /home/agustin/Descargas/test-miniature-03-08-5-city/city.avi.miniature.1.bmp 6
*/

const unsigned int gauss[5][5] = {  
    {1, 5, 18, 5, 1},
    {5, 32, 64, 32, 5},
    {18, 64, 100, 64, 18},
    {5, 32, 64, 32, 5},
    {1, 5, 18, 5, 1}
};

unsigned int get_miniature_color(unsigned char *src, int row_size, int row, int col) {
    unsigned char (*src_matrix)[row_size] = (unsigned char (*)[row_size]) src;
    unsigned int sum = 0;
    int i, j, r = row - 2, c = col - 6;

    for (i = 0; i < 5; i++) {
        c = col - 6;
        for (j = 0; j < 5; j++) {
            sum += ( (unsigned int)src_matrix[r][c] * (unsigned int)gauss[i][j]);
            c += 3;
        }
        r++;
    }

    sum = (unsigned int) (sum/600);
    return sum;
}

/*  topPlane:
        Numero entre 0 y 1 que representa el porcentaje de imagen desde el cual
        va a comenzar la primera iteración de blur (habia arriba)

    bottomPlane:
        Numero entre 0 y 1 que representa el porcentaje de imagen desde el cual
        va a comenzar la primera iteración de blur (hacia abajo)

    iters:
        Cantidad de iteraciones. Por cada iteración se reduce el tamaño de
        ventana que deben blurear, con el fin de generar un blur más intenso
        a medida que se aleja de la fila centro de la imagen.
*/
void miniature_c(
                unsigned char *src,
                unsigned char *dst,
                int width, int height,
                float topPlane, float bottomPlane,
                int iters) {

    unsigned char (*src_matrix)[width * 3] = (unsigned char (*)[width * 3]) src;
    unsigned char (*dst_matrix)[width * 3] = (unsigned char (*)[width * 3]) dst;
    int topPlaneLimit = topPlane * height;
    int bottomPlaneInit = bottomPlane * height;
    int newWidth = width * 3 - 6; //recorro hasta 2 pixels antes de terminar la matriz
    int row = 2; // filas empiezan en el 2do pixel
    int col = 6; // columnas empiezan en el 2do pixel
    int i, rowsToProcess;
    unsigned char b, g, r;


    /* La banda media queda igual */
    row = topPlaneLimit + 1;
    col = 6;
    while (row < bottomPlaneInit) {
        while (col <= newWidth) {
            dst_matrix[row][col] = src_matrix[row][col];
            dst_matrix[row][col + 1] = src_matrix[row][col + 1];
            dst_matrix[row][col + 2] = src_matrix[row][col + 2];
            col += 3;
        }
        col = 6;
        row++;
    }

    /* Loop iteraciones */
    for (i = 0; i < iters; i++) {

        /* Banda superior */
        rowsToProcess = i * topPlaneLimit / iters;
        rowsToProcess = topPlaneLimit - rowsToProcess;
        row = 2;
        while (row <= rowsToProcess) {
            while (col <= newWidth) {
                b = (unsigned char) get_miniature_color(src, width * 3, row, col);
                g = (unsigned char) get_miniature_color(src, width * 3, row, col + 1);
                r = (unsigned char) get_miniature_color(src, width * 3, row, col + 2);
                dst_matrix[row][col] = (unsigned char) b;
                dst_matrix[row][col + 1] = (unsigned char) g;
                dst_matrix[row][col + 2] = (unsigned char) r;

                col += 3;
            }
            col = 6;
            row++;
        }

        /* resultado lo copio a src para mayor blur */        
        row = 2;
        while (row <= rowsToProcess) {
            col = 6;
            while (col <= newWidth) {
                src_matrix[row][col] = dst_matrix[row][col];
                src_matrix[row][col + 1] = dst_matrix[row][col + 1];
                src_matrix[row][col + 2] = dst_matrix[row][col + 2];
                col += 3;
            }
            row++;
        }

        /* Banda inferior */
        rowsToProcess = i * (height - bottomPlaneInit) / iters;
        row = bottomPlaneInit + rowsToProcess;
        while (row <= height-2) {
            col = 6;
            while (col <= newWidth) {
                b = (unsigned char) get_miniature_color(src, width * 3, row, col);
                g = (unsigned char) get_miniature_color(src, width * 3, row, col + 1);
                r = (unsigned char) get_miniature_color(src, width * 3, row, col + 2);
                dst_matrix[row][col] = (unsigned char) b;
                dst_matrix[row][col + 1] = (unsigned char) g;
                dst_matrix[row][col + 2] = (unsigned char) r;

                col += 3;
            }
            row++;
        }

        /* resultado lo copio a src para mayor blur */        
        row = 2;
        while (row <= height-2) {
            col = 6;
            while (col <= newWidth) {
                src_matrix[row][col] = dst_matrix[row][col];
                src_matrix[row][col + 1] = dst_matrix[row][col + 1];
                src_matrix[row][col + 2] = dst_matrix[row][col + 2];
                col += 3;
            }
            row++;
        }
    }

}