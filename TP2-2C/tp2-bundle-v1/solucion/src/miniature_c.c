int get_miniature_color(unsigned char *src, int row_size, int row, int col) {
    unsigned char (*src_matrix)[row_size] = (unsigned char (*)[row_size]) src;
    int gauss[5][5] = {  
        {1, 5, 18, 5, 1},
        {5, 32, 64, 32, 5},
        {18, 64, 100, 64, 18},
        {5, 32, 64, 32, 5},
        {1, 5, 18, 5, 1}
    };
    int i, j, sum = 0, r = row - 2, c = col - 6;

    for (i = 0; i < 5; i++) {
        c = col - 6;
        for (j = 0; j < 5; j++) {
            sum += src_matrix[r][c] * gauss[i][j];
            c += 3;
        }
        r ++;
    }

    return sum/600;
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
    int newWidth = (width - 2) * 3; //recorro hasta 2 pixels antes de terminar la matriz
    int row = 2; // filas empiezan en el 2do pixel
    int col = 6; // columnas empiezan en el 2do pixel
    int i, b, g, r, rowsToProcess;

    /* Banda superior */
    for (i = 0; i < iters; i++) {
        rowsToProcess = (int) i * topPlaneLimit / iters;
        rowsToProcess = topPlaneLimit - rowsToProcess;
        row = 2;
        while (row <= rowsToProcess) {
            while (col <= newWidth) {
                b = get_miniature_color(src, width * 3, row, col);
                g = get_miniature_color(src, width * 3, row, col + 1);
                r = get_miniature_color(src, width * 3, row, col + 2);
                dst_matrix[row][col] = b;
                dst_matrix[row][col + 1] = g;
                dst_matrix[row][col + 2] = r;

                src_matrix[row][col] = b;
                src_matrix[row][col + 1] = g;
                src_matrix[row][col + 2] = r;

                col += 3;
            }
            col = 6;
            row++;
        }
    }

    /* La banda media queda igual */
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

    /* Banda inferior */
    for (i = 0; i < iters; i++) {
        rowsToProcess = (int) i * (height - bottomPlaneInit) / iters;
        row = bottomPlaneInit + rowsToProcess;
        col = 6;
        while (row < height) {
            while (col <= newWidth) {
                b = get_miniature_color(src, width * 3, row, col);
                g = get_miniature_color(src, width * 3, row, col + 1);
                r = get_miniature_color(src, width * 3, row, col + 2);
                dst_matrix[row][col] = b;
                dst_matrix[row][col + 1] = g;
                dst_matrix[row][col + 2] = r;

                src_matrix[row][col] = b;
                src_matrix[row][col + 1] = g;
                src_matrix[row][col + 2] = r;
                col += 3;
            }
            col = 6;
            row++;
        }
    }
}