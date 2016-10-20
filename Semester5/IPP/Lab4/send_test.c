//send_test.c
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

int main( int argc, char *argv[] )
{
    //Seed the rand function

    int i;
    int j;
    int rank, size;
    int potato;
    int dest;
    int arr[4][4] = {0};
    int potato_array[10] = {0};

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank==0) {
        potato = 1235687;
        MPI_Send(arr, 4, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("about to do the gather\n");
        MPI_Gather(&potato, 1, MPI_INT, potato_array, 1, MPI_INT, 0, MPI_COMM_WORLD);
        printf("%d", potato);
        for(i = 0; i<10; i++) {
        
            printf("potato_array: %d\n", potato_array[i]);
        }

        MPI_Bcast(potato_array, 10, MPI_INT, 0, MPI_COMM_WORLD);
    }

    else {
        potato = 1234;
        for(i = 0; i<10; i++) {
        
            printf("potato_array from slaves: %d\n", potato_array[i]);
        }

        MPI_Gather(&potato, 1, MPI_INT, potato_array, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(potato_array, 10, MPI_INT, 0, MPI_COMM_WORLD);
        for(i = 0; i<10; i++) {
        
            printf("potato_array from slaves: %d\n", potato_array[i]);
        }
    }


    MPI_Finalize();
}
