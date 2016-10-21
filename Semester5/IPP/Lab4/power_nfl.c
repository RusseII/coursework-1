#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void getTeamScores(int team, int scores[77][4], int teamScore[16][4]); 
float calculatePower(int localTeamScore[16][4], float powers[33]);

int main(int argc, char *argv[]  ) {
    int TEAM_SIZE = 32;
    //Initialize variables such as file pointer,
    //loop iterators and rank and size
    FILE *fp;
    char buff[255];
    char *teams[TEAM_SIZE];
    int i;
    int j;
    int rank, size;
    float summations = 0;
    float powers[33] = {100, 100, 100, 100, 100, 100, 100, 100,
                        100, 100, 100, 100, 100, 100, 100, 100, 
                        100, 100, 100, 100, 100, 100, 100, 100,
                        100, 100, 100, 100, 100, 100, 100, 100, 100};
    //Initialize a variable to store all of the scores
    int scores[77][4];
    int teamScores[16][4];
    int localTeamScore0[16][4];
    int localTeamScore1[16][4];
    int localTeamScore2[16][4];
    int localTeamScore3[16][4];
    
    float sum = 0;
    int otherTeam = 0;
    float localPower = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //If the rank is 0, that processes is the master 
    //The masters reads scores from the file and sends the 
    //correct information to each of the other processes
    if (rank == 0) {
        //Read the team names and save them
        fp = fopen("nfltms.dat", "r");
	for(i = 0; i < 32; i++) {
            fgets(buff, 255, (FILE*)fp);
            teams[i] = buff;
        }
        fclose(fp);
        
        //read the scores and then close it
        fp = fopen("xfl.dat", "r");
        for(i = 0; i < 77; i++) {
            fscanf(fp, "%d %d %d %d", &scores[i][0], &scores[i][1], &scores[i][2], &scores[i][3]);
            //printf("%d", scores[i][0]);
            //printf("\n");
        }
        fclose(fp);

        //Send each team its scores used to calculate the power function
        for(i = 1; i < 17; i++ ){
            getTeamScores(i, scores, teamScores);  
            MPI_Send(teamScores, 64, MPI_INT, i%8+1, 0, MPI_COMM_WORLD);
        }
        
        MPI_Barrier(MPI_COMM_WORLD);
    }
    
    else {
        sum = 0;
        summations = 0;
        MPI_Barrier(MPI_COMM_WORLD);

        MPI_Recv(localTeamScore0, 64, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        MPI_Recv(localTeamScore1, 64, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        MPI_Recv(localTeamScore2, 64, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        MPI_Recv(localTeamScore3, 64, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        //printf("about to receive. Rank: %d\n", rank);
        for(i = 0; i < 16; i++) {
            //:wprintf("localTeamScore: %d %d %d %d\n", localTeamScore[i][0], localTeamScore[i][1], localTeamScore[i][2], localTeamScore[i][3]); 
        }

        localPower = calculatePower(localTeamScore1, powers);
            

        //for(i = 0; i < 16; i++) {
        //    if (localTeamScore0[i][0] == rank) {
        //        otherTeam = localTeamScore0[i][1];
        //        sum = sum + localTeamScore0[i][2] - localTeamScore0[i][3];
        //        summations += powers[otherTeam];
        //    }
        //    else {
        //        otherTeam = localTeamScore0[i][0];
        //        sum = sum + localTeamScore0[i][3] - localTeamScore0[i][2];
        //        summations += powers[otherTeam];
        //    }
        //}
        //if(rank == 1) {
        //    printf("sum: %f\n\n", sum);        
        //    printf("localPower: %f\n\n", localPower);        
        //    localPower = (summations + sum) / 16;
        //    printf("localPowerPOwer: %f\n\n", localPower);        
        //}
        //else 
        //    localPower = (summations + sum) / 16;



        //make equation
        //calculate with 100 as intial value
        //MPI gaterh
        MPI_Gather(&localPower, 1, MPI_FLOAT, powers, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    }

    int tol = 0;
    while (tol < 10) {
        if (rank == 0){
            localPower = 110;
                printf("Powers: ");
            for(i = 0; i < 17; i++)
                printf("%f ,", powers[i]);
            printf("\n");
            MPI_Gather(&localPower, 1, MPI_FLOAT, powers, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
            MPI_Bcast(powers, 17, MPI_FLOAT, 0, MPI_COMM_WORLD);
        }

        else {
            sum = 0;
            summations = 0;
            MPI_Bcast(powers, 17, MPI_FLOAT, 0, MPI_COMM_WORLD);
            for(i = 0; i < 16; i++) {
                if (localTeamScore[i][0] == rank) {
                    otherTeam = localTeamScore[i][1];
                    sum += localTeamScore[i][2] - localTeamScore[i][3];
                    summations += powers[otherTeam];
                }
                else {
                    otherTeam = localTeamScore[i][0];
                    sum += localTeamScore[i][3] - localTeamScore[i][2];
                    summations += powers[otherTeam];
                }
            }
            localPower = (summations + sum) / 16;
            MPI_Gather(&localPower, 1, MPI_FLOAT, powers, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
        }
        tol = tol +1;
    }
    MPI_Finalize();
    return 0;
}

void getTeamScores(int team, int scores[77][4], int teamScores[16][4]) {
   int i;
   int j;
   int count = 0;
   for(i = 0; i < 77; i++) {
        if(scores[i][0] == team || scores[i][1] == team  ) {
            for(j = 0; j<4; j++){
                teamScores[count][j] = scores[i][j];
            }
            count++;
        }  
   }
}
        float calculatePower(int localTeamScore[16][4], float powers[33]) {
            sum = 0;
            summations = 0;
            for(i = 0; i < 16; i++) {
                if (localTeamScore[i][0] == rank) {
                    otherTeam = localTeamScore[i][1];
                    sum = sum + localTeamScore[i][2] - localTeamScore[i][3];
                    summations += powers[otherTeam];
                }
                else {
                    otherTeam = localTeamScore[i][0];
                    sum = sum + localTeamScore[i][3] - localTeamScore[i][2];
                    summations += powers[otherTeam];
                }
            }
            localPower = (summations + sum) / 16;
            return localPower;
        }
