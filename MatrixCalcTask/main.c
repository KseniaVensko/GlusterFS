#include <stdio.h>
#include <stdlib.h>     // malloc
#include <errno.h>
#include <math.h>
#include <fcntl.h>
#define COLUMNS 18
#define ROWS 22

// Usage: ./MatrixCalcTask inputMatrixFile, rank (rank is power of 2)

void readMatrix(FILE* input, double** matrix) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            double temp;
            fscanf(input, "%lf", &temp);
            matrix[i][j] = temp;
        }
    }
}

void convertMatrix(double** matrix, int rank) {
    printf("rank %i\n", rank);
    int dim = 1 << rank;
    double average = 0;
    int count=0;
    printf("dim %i\n", dim);
    int rows = (ROWS + dim - 1) / dim;              // ceil (ROWS / 4)
    int columns = (COLUMNS + dim - 1) / dim;
    printf("rows %i\n", rows);
    printf("columns %i\n", columns);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            for (int k = 0; k < dim; ++k) {
                for (int p = 0; p < dim; ++p) {
                    if ((i*dim+k < ROWS) && (j*dim+p < COLUMNS)) {
                        average += matrix[i*dim+k][j*dim+p];
                        count++;
                    }
                }
            }
            average /= count;
            for (int z = 0; z < dim; ++z) {
                for (int x = 0; x < dim; ++x) {
                    if ((i*dim+z < ROWS) && (j*dim+x < COLUMNS)) {
                        matrix[i*dim+z][j*dim+x] = average;
                    }
                }
            }
            average=count=0;
        }
    }
}

void writeMatrix(FILE* output, double** matrix) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            fprintf(output, "%lf ", matrix[i][j]);
        }
        fprintf(output, "\n");
    }
}

int grantLock(FILE* file) {
    int fd = fileno(file);
    struct flock lock;
    lock.l_whence = SEEK_SET; // Абсолютное положение
    lock.l_start = 0; // /* Начало файла */
    lock.l_len = 0; // /* До конца файла */
    lock.l_type = F_WRLCK | F_RDLCK;

    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        printf("So sad: %s\n", strerror(errno));
        return 0;
    }
    else { /* Lock was granted... */
        return 1;
    }
}

int releaseLock(FILE* file) {
    int fd = fileno(file);
    struct flock lock;

    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        printf("Can not free: %s\n", strerror(errno));
        return 0;
    }
    else {
        return 1;
    }
}

int main(int argc, char *argv[])
{
    double** matrix = (double **) malloc(sizeof(double*) * ROWS);

    for (int i = 0; i < ROWS; i++) {
        matrix[i] = (double*) malloc(sizeof(double) * COLUMNS);
    }

    FILE* in = fopen(argv[1], "a+");
    if ( in == NULL ) {
        printf("%s %s : %s\n", "Cannot open input file", argv[1], strerror(errno));
        return 0;
    }
    int rank = atoi(argv[2]);

    if (grantLock(in)) {
        readMatrix(in, matrix);         // what if we broke here?
        releaseLock(in);
    }

    if (fclose(in) == -1) {
        printf("Can not close input file");
    }

    convertMatrix(matrix, rank);

    FILE* out = fopen(argv[1], "a+");

    if ( out == NULL ) {
        printf("%s %s : %s\n", "Cannot open output file", argv[1], strerror(errno));
        return 0;
    }

    if (grantLock(out)) {
        writeMatrix(out, matrix);         // what if we broke here?
        releaseLock(out);
    }

    if (fclose(out) == -1) {
        printf("Can not close input file");
    }

    for (int i = 0; i < ROWS; i++) {
        free(matrix[i]);
    }

    free(matrix);
    return 0;
}
