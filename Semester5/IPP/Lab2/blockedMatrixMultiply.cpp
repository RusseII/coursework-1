#include <iostream>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    int matrix_size = 1024;
    int block_size = 1024;

    short A [matrix_size][matrix_size];
    for (int i = 0; i < matrix_size; i++)
        for (int j = 0; j < matrix_size; j++)
            A[i][j] = 1;

    short B [matrix_size][matrix_size];
    for (int i = 0; i < matrix_size; i++)
        for (int j = 0; j < matrix_size; j++)
            B[i][j] = 1;

    short C [matrix_size][matrix_size];
    for (int i = 0; i < matrix_size; i++)
        for (int j = 0; j < matrix_size; j++)
            B[i][j] = 1;

    struct timeval start, end;

    gettimeofday(&start, NULL);

    for (int k = 0; k < matrix_size; k += block_size)
      for (int j = 0; j < matrix_size; j += block_size)
        for (int i = 0; i < matrix_size; i++)
            for (int jj = j; jj < std::min(j + block_size, matrix_size); jj++)
                for (int kk = k; kk < std::min(k + block_size, matrix_size); kk++)
                    C[i][jj] = C[i][jj] + A[i][kk] * B[kk][jj];

    gettimeofday(&end, NULL);
    std::cout << (end.tv_sec*1000000 + end.tv_usec) - (start.tv_sec*1000000 + start.tv_usec) << std::endl;
}
