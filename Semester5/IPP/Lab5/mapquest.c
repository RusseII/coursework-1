#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
    int rank,
        size,
        i;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    
    FILE *fp;
    char buff[255];

    if (rank == 0) {
        //Read the team names and save them
        int cities = 0;
        fp = fopen("nqmqSmall.dat", "r");
        fscanf(fp, "%d", &cities);
        char *citiesNames[cities];
        
	for(i = 1; i <= cities; i++) {
            fgets(buff, 255, (FILE*)fp);
            citiesNames[i] = buff;
            printf("%d. %s", i, citiesNames[i]);
        }

        fclose(fp);


        for(i = 0; i < cities; i++) {
            printf("%s", citiesNames[i]);
        }
    }
    MPI_Finalize();

    return 0;
}
