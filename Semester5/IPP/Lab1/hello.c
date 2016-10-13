/* Part 1. OpenMP test
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int thread_count;

void hello() {
   int my_rank = omp_get_thread_num();
   thread_count = omp_get_num_threads();
   printf("Hello world from thread %d of %d.\n", my_rank, thread_count);
}

int main(int argc, char* argv[]) {
   thread_count = strtol(argv[1], NULL, 10);
  
#  pragma omp parallel num_threads(thread_count)
   hello();
   
   return 0;
}
// end Part 1

*/
// Part 2. MPI test
#include <stdio.h>
#include <mpi.h>

void hello() {
   int my_rank, thread_count;
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   MPI_Comm_size(MPI_COMM_WORLD, &thread_count);
   printf("Hello world from thread %d of %d.\n", my_rank, thread_count);
}

int main(int argc, char* argv[]) {
   MPI_Init(&argc, &argv);
   hello();
   MPI_Finalize();
   
   return 0;
}
// end Part 2
