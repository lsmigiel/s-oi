#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <stdbool.h>
#include <time.h>
// #include "data.h"
#include "api.c"

int main(){
    int i;
    message m;
    srand(time(NULL));

    queue *shm;
    getMemory(&shm);

    while(true){

        printf("QUEUE_A:    QUEUE_B:    QUEUE_C:\n");
        for(i=0; i<20; i++){
            m = shm[QUEUE_A].messages[i];
            printf("(%2d, %c%c%c)    ", m.priority, m.letter1, m.letter2, m.letter3);
            m = shm[QUEUE_B].messages[i];            
            printf("(%2d, %c%c%c)    ", m.priority, m.letter1, m.letter2, m.letter3);
            m = shm[QUEUE_C].messages[i];            
            printf("(%2d, %c%c%c)\n", m.priority, m.letter1, m.letter2, m.letter3);
        } 

        sleep(1);

    }

    

    return 0;
}




