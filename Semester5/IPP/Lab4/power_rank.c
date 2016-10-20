#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void getTeamScores(int team, int scores[40][4], int teamScore[10][4]); 

int main(int argc, char *argv[]  ) {
    int TEAM_SIZE = 8;
    //Initialize variables such as file pointer,
    //loop iterators and rank and size
    FILE *fp;
    char buff[255];
    char *teams[TEAM_SIZE];
    int i;
    int j;
    int rank, size;
    int powers[9] = {100, 100, 100, 100, 100, 100, 100, 100,
                                100};
    //Initialize a variable to store all of the scores
    int scores[40][4];
    int teamScores[10][4];
    int localTeamScore[10][4];
    
    int sum = 0;
    int otherTeam = 0;
    int localPower = 0;

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
        for(i = 0; i < 9; i++ ){
            getTeamScores(i, scores, teamScores);  
            printf("sending...\n");
            MPI_Send(teamScores, 40, MPI_INT, i, i, MPI_COMM_WORLD);
        }
        
        MPI_Barrier(MPI_COMM_WORLD);
    }
    
    else {
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Recv(localTeamScore, 40, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        printf("about to receive. Rank: %d\n", rank);
        for(i = 0; i < 10; i++) {
            printf("localTeamScore: %d %d %d %d\n", localTeamScore[i][0], localTeamScore[i][1], localTeamScore[i][2], localTeamScore[i][3]); 
        }

        for(i = 0; i < 10; i++) {
            if (localTeamScore[i][0] == rank) {
                otherTeam = localTeamScore[i][1];
                sum = sum + localTeamScore[i][2] - localTeamScore[i][3];
                localPower += powers[otherTeam];
            }
            else {
                otherTeam = localTeamScore[i][0];
                sum = sum + localTeamScore[i][3] - localTeamScore[i][2];
                localPower += powers[otherTeam];
            }
        }
        if(rank == 1) {
            printf("sum: %d\n\n", sum);        
            printf("localPower: %d\n\n", localPower);        
            localPower = (localPower + sum) / 10;
            printf("localPowerPOwer: %d\n\n", localPower);        
        }
        else 
            localPower = (localPower + sum) / 10;



        //make equation
        //calculate with 100 as intial value
        //MPI gaterh
        MPI_Gather(&localPower, 1, MPI_INT, powers, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }

    int tol = 0;
    while (tol < 10) {
        if (rank == 0){
            localPower = 0;
                printf("Powers: ");
            for(i = 0; i < 9; i++)
                printf("%d ,", powers[i]);
            printf("\n");
            MPI_Gather(&localPower, 1, MPI_INT, powers, 1, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(powers, 9, MPI_INT, 0, MPI_COMM_WORLD);
        }

        else {
            MPI_Bcast(powers, 9, MPI_INT, 0, MPI_COMM_WORLD);
            for(i = 0; i < 10; i++) {
                if (localTeamScore[i][0] == rank) {
                    otherTeam = localTeamScore[i][1];
                    sum += localTeamScore[i][2] - localTeamScore[i][3];
                    localPower += powers[otherTeam];
                }
                else {
                    otherTeam = localTeamScore[i][0];
                    sum += localTeamScore[i][3] - localTeamScore[i][2];
                    localPower += powers[otherTeam];
                }
            }
            localPower = (localPower + sum) / 10;
            MPI_Gather(&localPower, 1, MPI_INT, powers, 1, MPI_INT, 0, MPI_COMM_WORLD);
        }
        tol = tol +1;
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
        if(scores[i][0] == team || scores[i][1] ==team  ) {
            for(j = 0; j<4; j++){
                teamScores[count][j] = scores[i][j];
            }
            count++;
        }  
   }
}

void recvAndCompute(int team, int TeamScore[10][4]) {



}
