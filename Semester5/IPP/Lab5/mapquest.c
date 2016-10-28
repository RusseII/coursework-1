#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define DATA_LINE_MAX_LEN 80

int num_threads = 1;
int cities = 0;

int dist[8][8];
int thro[8][8];
char *citiesNames[8][200];
int INT_MAX = 1000000;
void calculate_paths(int cities, int num_threads, int thro[8][8], int dist[8][8]); 
void path_direction(int a, int b, int thro[8][8], int dist[8][8], char *citiesNames[8][200]); 

int main(int argc, char *argv[]) {
    int rank, size, i, j;
    int cities = 0;
    FILE *fp;
    char buff[1024];
    char line[DATA_LINE_MAX_LEN];

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
    		thro[i][j]   = -1;
    	}
    }

    for (i = 0; i < cities;  ++i)
        dist[i][i] = 0;

    int total;
    for (total = 0; 1; ++total) {
    	int city_a = 0, city_b = 0, distance   = 0;
    
        //Read in the next data points
    	fscanf(fp, "%d %d %d", &city_a, &city_b, &distance);
    	fgets(line, DATA_LINE_MAX_LEN, fp);
    
    	if (city_a == -1)
    		break;
    
        //Set the distance from the data points
    	dist[city_a][city_b] = distance;
    	dist[city_b][city_a] = distance;
    }
    
    fclose(fp);

    //Get the thread number from command line arguments
    if (argc > 1) {
    	num_threads = atoi(argv[1]);
    }
    
    //Check if the number of threads is divisible by cities
    if (cities % num_threads != 0) {
    	printf("%d threads is not evenly divisible into %d cities\n", num_threads, cities);
    	exit(1);
    }
    
    #pragma omp parallel num_threads(num_threads)
    calculate_shortest_paths(cities, num_threads, thro, dist);


    //Interface with user
    char ans = 'y';
    while (ans == 'y') {
        printf("NQMQ Menu\n");
        
        // Display all cities with numbers
        for (i = 0; i < cities; ++i) {
        	printf("%2d. %s\n", i + 1, citiesNames[i]);
        }
        
        int start, end;
        
        printf("\nPath from: ");
        scanf("%d", &start);
        printf("Path to: ");
        scanf("%d", &end);
        start = start -1;
        end = end -1;
        
        printf("%s to %s\n", citiesNames[start], citiesNames[end]);
        
        //Check if it is a max distance
        if (dist[start][end] == INT_MAX) {
        	printf("No path available between these cities");
        	exit(1);
        }
        
        //print out the path from the two cities with the through matrix
        path_direction(start, end, thro, dist,  citiesNames);
        printf("Total Distance: %d miles\n", dist[start][end]);

        printf("\nCalculate another distance? y/n: ");
        scanf(" %c", &ans);
    }
    
    return 0;
}

void calculate_shortest_paths(int cities, int num_threads, int thro[8][8], int dist[8][8]) {
    int id = omp_get_thread_num();
    int k;
    int i;
    int j;
    
    // Determine the rows we will be calculating
    int start = (id * cities) / num_threads;
    int end   = (id + 1) * cities / num_threads;
    
    for (k = 0; k < cities; ++k)
    {
    	// Wait for all threads before we do shortest path calculations on the
    	// adjacency matrix for this intermediate city
    	#pragma omp barrier
    
    	for (i = start; i < end; ++i) {
            for (j = 0; j < cities; ++j) {
                // Ignore any city to itself and ignore any max paths
                if (dist[i][k] == INT_MAX || dist[k][j] == INT_MAX || i == j)
                    continue;
    
                // Check if there is a faster path through node k
                int new_dist = dist[i][k] + dist[k][j];
                if (dist[i][j] <= new_dist)
                    continue;
    
                // This way is faster! Update the min distance and keep track of
                // the node that we can travel through to get here
                dist[i][j] = new_dist;
                thro[i][j] = k;
            }
    	}
    }
}
void path_direction(int a, int b, int thro[8][8], int dist[8][8], char *citiesNames[8][200]) {
	int intermediate = thro[a][b];
	if (intermediate == -1) {
		printf("%s -> %s\n", citiesNames[a], citiesNames[b]);
	}

	else {
		path_direction(a, intermediate, thro, dist, citiesNames);
		path_direction(intermediate, b, thro, dist, citiesNames);
	}
}
