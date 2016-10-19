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

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank==0) {
        MPI_Send(arr, 4, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }

    else if (rank == 1) {
        MPI_Recv(arr, 4, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (i =0; i < 4; i++ ) {
            for (j =0; j< 4; j++ ) {
                printf("Val in rank 1: %d\n", arr[i][j]);
            }
        }
        
    }


    MPI_Finalize();
}
