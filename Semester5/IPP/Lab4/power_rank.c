#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void getTeamScores(int team, int scores[40][4], int teamScore[10][4]); 

int main(int argc, char *argv[]  ) {
    //Initialize variables such as file pointer,
    //loop iterators and rank and size
    FILE *fp;
    char buff[255];
    char *teams[8];
    int i;
    int j;
    int rank, size;
    //Initialize a variable to store all of the scores
    int scores[40][4];
    //Initialize 8 more team scores, one for each process
    int teamScore1[10][4];
    int teamScore2[10][4];
    int teamScore3[10][4];
    int teamScore4[10][4];
    int teamScore5[10][4];
    int teamScore6[10][4];
    int teamScore7[10][4];
    int teamScore8[10][4];
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //If the rank is 0, that processes is the master 
    //The masters reads scores from the file and sends the 
    //correct information to each of the other processes
    if (rank == 0) {
        //Read the team names and save them
        fp = fopen("xfltms.dat", "r");
	for(i = 0; i < 8; i++) {
            fgets(buff, 255, (FILE*)fp);
            teams[i] = buff;
        }
        fclose(fp);
        
        //read the scores and then close it
        fp = fopen("xfl.dat", "r");
        for(i = 0; i < 40; i++) {
            fscanf(fp, "%d %d %d %d", &scores[i][0], &scores[i][1], &scores[i][2], &scores[i][3]);
            //printf("%d", scores[i][0]);
            //printf("\n");
        }
        fclose(fp);

        //Send each team its scores used to calculate the power function
        getTeamScores(1, scores, teamScore1);
        MPI_Send(teamScore1, 40, MPI_INT, 1, 0, MPI_COMM_WORLD);

        getTeamScores(2, scores, teamScore2);
        MPI_Send(teamScore1, 40, MPI_INT, 2, 0, MPI_COMM_WORLD);

        getTeamScores(3, scores, teamScore3);
        MPI_Send(teamScore1, 40, MPI_INT, 3, 0, MPI_COMM_WORLD);

        getTeamScores(4, scores, teamScore4);
        MPI_Send(teamScore1, 40, MPI_INT, 4, 0, MPI_COMM_WORLD);

        getTeamScores(5, scores, teamScore5);
        MPI_Send(teamScore1, 40, MPI_INT, 5, 0, MPI_COMM_WORLD);

        getTeamScores(6, scores, teamScore6);
        MPI_Send(teamScore1, 40, MPI_INT, 6, 0, MPI_COMM_WORLD);

        getTeamScores(7, scores, teamScore7);
        MPI_Send(teamScore1, 40, MPI_INT, 7, 0, MPI_COMM_WORLD);

        getTeamScores(8, scores, teamScore8);
        MPI_Send(teamScore1, 40, MPI_INT, 8, 0, MPI_COMM_WORLD);
    }


    //I need to receive all of the team scores
    //Computer power 1 for rank 1's team
    //and send it to the other processes
    //Then compute again and send again
    //compute send
    //compute
    //send
    //until the relative error is below 0.05
    
    else {
        printf("Aout to recieve!\n");
        MPI_Recv(teamScore1, 40, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        for(i = 0; i < 10; i++) {
                printf("FromTeam1: %d %d %d %d\n", teamScore1[i][0], teamScore1[i][1], teamScore1[i][2], teamScore1[i][3]); 
        }
    }
    
    MPI_Finalize();
    return 0;
}

void getTeamScores(int team, int scores[40][4], int teamScores[10][4]) {
   int i;
   int j;
   int count = 0;
   //printf("HAHA HERE IS THE TEAM: %d, ", team);
   //int teamScores[10][4];
   for(i = 0; i < 40; i++) {
        if(scores[i][0] == 1 || scores[i][1] ==1  ) {
            for(j = 0; j<4; j++){
                teamScores[count][j] = scores[i][j];
            }
            count++;
        }  
   }
}

void recvAndCompute(int team, int TeamScore[10][4]) {



}
