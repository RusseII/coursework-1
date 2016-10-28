#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
//#include <limits.h>
#include <time.h>
#include <sys/time.h>


#define DATA_LINE_MAX_LEN 80
int num_threads = 1;
int cities = 0;

int dist[8][8];
int thro[8][8];
    char *citiesNames[8][200];
    int INT_MAX = 1000000;

void print_path_directions(int a, int b, int thro[8][8], int dist[8][8], char *citiesNames[8][200])
{
	int intermediate = thro[a][b];

	if (intermediate == -1)
	{
		printf("    %s \e[0;32m->\e[0m %s (%d miles)\n", citiesNames[a], citiesNames[b], dist[a][b]);
	}
	else
	{
                //printf("does this print\n");
		print_path_directions(a, intermediate, thro, dist, citiesNames);
		print_path_directions(intermediate, b, thro, dist, citiesNames);

	}
}

void calculate_shortest_paths(int cities, int num_threads, int thro[8][8], int dist[8][8])
{
	int id = omp_get_thread_num();

	// Determine the rows we will be calculating
	int start = id * cities / num_threads;
	int end   = (id + 1) * cities / num_threads;

	for (int k = 0; k < cities; ++k)
	{
		// Wait for all threads before we do shortest path calculations on the
		// adjacency matrix for this intermediate city
		#pragma omp barrier

		for (int i = start; i < end; ++i)
		{
			for (int j = 0; j < cities; ++j)
			{
				// If The k node or target node is infinity (INT_MAX) then
				// we are just going to overflow the numbers. Ignore them.
				// We can also safely ignore cities between to themselves
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

	printf("    \e[0;33mThread-%d \e[0;32m->\e[0m Finished all calculations for rows %2d => %2d\n",
		id, start + 1, end);
}

int main(int argc, char *argv[]) {
    int rank,
        size,
        i,
        j;

    
    FILE *fp;
    char buff[1024];
    char line[DATA_LINE_MAX_LEN];


    //Read the team names and save them
    int cities = 0;
    fp = fopen("nqmqSmall.dat", "r");
    fscanf(fp, "%d", &cities);
    
    int dist[cities][cities];
    int thro[cities][cities];

    for(i = 0; i < 9; i++) {
        fgets(buff, 1024, (FILE*)fp);
        printf("%s", buff);
        if(i != 0)
            strcpy(citiesNames[i-1],buff);
    }
    printf("THiS shoudl BE THE firtst one: %s", citiesNames[0]);

    //Get the user input
    for(i = 0; i < 8; i++) 
        printf("%d. %s", i,  citiesNames[i]);

    for (int i = 0; i < cities; ++i)
    {
    	for (int j = 0; j < cities; ++j)
    	{
    		dist[i][j] = INT_MAX;
    		thro[i][j]   = -1;
    	}
    }

    for (int i = 0; i < cities; dist[i][i] = 0, ++i);
    int total;
    for (total = 0; 1; ++total)
    {
    	int city_a = 0,
    	    city_b = 0,
    	    distance   = 0;
    
    	fscanf(fp, "%d %d %d", &city_a, &city_b, &distance);
        printf("%d %d %d\n", city_a, city_b, distance);
    	fgets(line, DATA_LINE_MAX_LEN, fp);
    
    	if (city_a == -1)
    		break;
    
    	// The cities are _NOT_ zero indexed in the data file
    	dist[city_a][city_b] = distance;
    	dist[city_b][city_a] = distance;

    }
    
    printf("    Read in %d connecting roads with distances...\n", total);
    fclose(fp);

    if (argc > 1)
    {
    	num_threads = atoi(argv[1]);
    }
    
    if (cities % num_threads != 0)
    {
    	printf("\e[0;31m==> %d threads is not evenly divisible into %d cities\n",
    		num_threads, cities);
    	exit(1);
    }
    
    printf("\e[0;34m==>\e[0m Starting up %d threads to calculate shortest paths...\n", num_threads);
    
    struct timeval time_start;
    struct timeval time_end;
    printf("BEFORE CALCS\n");
    for(i = 0; i < 8; i++){
        for(j = 0; j < 8; j++) {
            printf("%d\t", dist[i][j]);
        }
        printf("\n");
    }
    
    gettimeofday(&time_start, NULL);
    #pragma omp parallel num_threads(num_threads)
        calculate_shortest_paths(cities, num_threads, thro, dist);

    printf("After cals\n");
    for(i = 0; i < 8; i++){
        for(j = 0; j < 8; j++) {
            printf("%d\t", dist[i][j]);
        }
        printf("\n");
    }

    gettimeofday(&time_end, NULL);
    // Display the menu
    puts("NQMQ Menu");
    puts("---------");
    puts("");
    
    // Display all cities with numbers
    for (int i = 0; i < cities; ++i)
    {
    	printf("%2d. %s\n", i + 1, citiesNames[i]);
    }
    
    int start, end;
    
    printf("\nPath from: ");
    scanf("%d", &start);
    printf("Path to: ");
    scanf("%d", &end);
    
    // These need to be zero indexed
    --start; --end;
    printf("\n\e[0;36m==>\e[0m %s to %s:\n\n", citiesNames[start], citiesNames[end]);
    
    if (dist[start][end] == INT_MAX)
    {
    	printf("\e[0;31m==> No path available between these cities");
    	exit(1);
    }
    
    print_path_directions(start, end, thro, dist,  citiesNames);
    printf("\n\e[0;32m==>\e[0m Total Distance: %d miles\n", dist[start][end]);
    
    // Free up the cities array and distances array
    //for (int i = 0; i < cities; free(citiesNames[i++]));
    //free(citiesNames);
    //free(dist);

    return 0;
}
