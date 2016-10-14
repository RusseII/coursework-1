#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void sendGames(int team, int scores[40][4], int teamScore[10][4]); 

int main(int argc, char *argv[]  ) {
    FILE *fp;
    char buff[255];
    char *teams[8];
    int rank, size;
    int scores[40][4];
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

    if (rank == 0) {
        fp = fopen("xfltms.dat", "r");

        int i;
        for(i = 0; i < 8; i++) {
            fgets(buff, 255, (FILE*)fp);
            //fscanf(fp, "%s", teams[i]);
            teams[i] = buff;
            printf(teams[i]);
        }
        fclose(fp);
        
        fp = fopen("xfl.dat", "r");
        for(i = 0; i < 40; i++) {
            fscanf(fp, "%d %d %d %d", &scores[i][0], &scores[i][1], &scores[i][2], &scores[i][3]);
            printf("%d", scores[i][0]);
            printf("\n");
        }
        fclose(fp);

        sendGames(1, scores, teamScore1);
        sendGames(2, scores, teamScore2);
        sendGames(3, scores, teamScore3);
        sendGames(4, scores, teamScore4);
        sendGames(5, scores, teamScore5);
        sendGames(6, scores, teamScore6);
        sendGames(7, scores, teamScore7);
        sendGames(8, scores, teamScore8);
    }

    
    MPI_Finalize();
    return 0;
}





void sendGames(int team, int scores[40][4], int teamScores[10][4]) {
   int i;
   int j;
   //int teamScores[10][4];
   for(i = 0; i < 40; i++) {
        if(scores[i][0] == team || scores[i][1] == team) {
            for(j = 0; j<4; j++){
                teamScores[i][j] = scores[i][j];
            }
        }  
   }
}
