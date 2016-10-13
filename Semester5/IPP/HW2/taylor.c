#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int factorial(int f) {
    if (f == 0)
        return 1;
    return (f * factorial(f-1));
}

int main(int argc, char *argv[] ) {
    int rank, size;
    //Convert the command line character array to integers
    //used for the value x
    int x = atoi(argv[1]);

    //Float variables for the local, and global sums 
    float global_sum, local_sum, total_val;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    local_sum = (pow(x, rank+2) / factorial(rank + 2));
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        total_val = 1 + x + global_sum;
        printf("Total Value: %9.6f\n", total_val);
    }
    
    MPI_Finalize();
    return 0;
}
