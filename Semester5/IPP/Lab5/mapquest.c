#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>

int num_threads = 1;
int cities = 0;

int dist[8][8];
int thro[8][8];
char *citiesNames[8][200];
int INT_MAX = 1000000;
void floyd(int cities, int num_threads, int thro[8][8], int dist[8][8]); 
void path(int a, int b, int thro[8][8], int dist[8][8], char *citiesNames[8][200]); 

int main(int argc, char *argv[]) {
    int rank, size, i, j;
    int cities = 0;
    FILE *fp;
    char buff[1024];
    char line[80];

    //Read in the number of cities first
    fp = fopen("nqmqSmall.dat", "r");
    fscanf(fp, "%d", &cities);
    
    for(i = 0; i < 9; i++) {
        fgets(buff, 1024, (FILE*)fp);
        if(i != 0)
            strcpy(citiesNames[i-1],buff);
    }

    //Set them initially to int max and the through matrix to be all -1s
    for (i = 0; i < cities; ++i) {
    	for (int j = 0; j < cities; ++j) {
    		dist[i][j] = INT_MAX;
                if(i == j) {
                    dist[i][j] = 0;
                }
    		thro[i][j]   = -1;
    	}
    }

    //Run until -1 is read
    while(true) {
    	int a = 0, b = 0, distance = 0;
    
        //Read in the next data points
    	fscanf(fp, "%d %d %d", &a, &b, &distance);
    	fgets(line, 80, fp);
    
    	if (a == -1)
    		break;
    
        //Set the distance from the data points
    	dist[a][b] = distance;
    	dist[b][a] = distance;
    }
    
    fclose(fp);

    //Get number of threads
    if (argc > 1) {
    	num_threads = atoi(argv[1]);
    }

    struct timeval time_start;
    struct timeval time_end;

    gettimeofday(&time_start, NULL);

    
    #pragma omp parallel num_threads(num_threads)
        floyd(cities, num_threads, thro, dist);

    gettimeofday(&time_end, NULL);


long long execution_time = 10000 * (time_end.tv_sec  - time_start.tv_sec) + (time_end.tv_usec - time_start.tv_usec);

	printf("Took: %lld microseconds.\n\n", execution_time);


    //Interface with user
    //and ask if they want to run again
    char ans = 'y';
    while (ans == 'y') {
        printf("NQMQ Menu\n");
        
        for (i = 0; i < cities; i++) {
        	printf("%d. %s\n", i + 1, citiesNames[i]);
        }
        
        int start, end;
        
        printf("\nFrom: ");
        scanf("%d", &start);
        printf("To: ");
        scanf("%d", &end);
        start = start -1;
        end = end -1;
        
        printf("%s to %s\n", citiesNames[start], citiesNames[end]);
        
        //Check if it is a max distance
        if (dist[start][end] == INT_MAX) {
        	printf("No path between cities");
        	exit(1);
        }
        
        //print out the path from the two cities with the through matrix
        path(start, end, thro, dist,  citiesNames);

        //Print out the distance
        printf("Distance: %d miles\n", dist[start][end]);
        printf("\nCalculate another distance? y/n: ");
        scanf(" %c", &ans);
    }
    
    return 0;
}

void floyd(int cities, int num_threads, int thro[8][8], int dist[8][8]) {
    int id = omp_get_thread_num();
    int k;
    int i;
    int j;
    
    int start = (id * cities) / num_threads;
    int end   = (id + 1) * cities / num_threads;
    
    for (k = 0; k < cities; k++) {
    	//#pragma omp barrier
    	for (i = start; i < end; i++) {
            for (j = 0; j < cities; j++) {
                // Ignore any city to itself and ignore any max paths
                if (dist[i][k] == INT_MAX || dist[k][j] == INT_MAX || i == j)
                    continue;
    
                int new_dist = dist[i][k] + dist[k][j];
                if (dist[i][j] <= new_dist)
                    continue;
    
                //update min distance
                dist[i][j] = new_dist;
                thro[i][j] = k;
            }
    	}
    }
}
void path(int a, int b, int thro[8][8], int dist[8][8], char *citiesNames[8][200]) {
	int intermediate = thro[a][b];
	if (intermediate == -1) {
		printf("%s -> %s\n", citiesNames[a], citiesNames[b]);
	}

	else {
		path(a, intermediate, thro, dist, citiesNames);
		path(intermediate, b, thro, dist, citiesNames);
	}
}
