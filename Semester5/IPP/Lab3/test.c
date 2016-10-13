#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

void delay(unsigned int mseconds) {
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

int main()  {


    while (true) {
        
        srand(time(NULL));
        printf("#: %d\n", rand()%4);
        delay(400000);
    
    }

}
