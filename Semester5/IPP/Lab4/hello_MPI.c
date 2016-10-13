#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

int main( int argc, char *argv[] )
{
    //Seed the rand function
    srand(time(NULL));

    int rank, size;
    int potato;
    int dest;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //Master process (0) sends the initial potato
    if (rank == 0) {
        potato = (rand() % 10) + size;
        dest = rand() % size;
        MPI_Send(&potato, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
        printf("Node %d has the potato, passing to node %d\n", rank, potato, dest);
    }

    //While potato does not equal the stop condition
    while (potato != -1) {
        //Wait for a message to be sent to this porcess
        MPI_Recv(&potato, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        if (potato == -1)
            break;

        potato = potato -1; 

        //Game over condition
        if (potato == 0) {
            printf("Node %d is it, game over.\n", rank); 
            int end = -1;
            int i;
            //Send an end message to all processes
            for (i = 0; i < size; i++){
                MPI_Send(&end, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
            potato = -1;
        }
        //If the game is not over, generate new random number and send the potato
        else {
            dest = rand()%size;
            MPI_Send(&potato, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
            printf("Node %d has the potato, passing to node %d\n", rank, potato,  dest);
        }
    }
    MPI_Finalize();
    return 0;
}
