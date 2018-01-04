#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <stdbool.h>
#include <time.h>
#include "api.c"

//running: ./producer queueNumber
int main(int argc, char *argv[])
{
    if(argc != 2){printf("Specify queue nnumber: -1 for random queue, 0-2 for concrete\n"); return 0;}    

    int queueNumber;
    int mode =  atoi(argv[1]);
    queueNumber = mode;
    
    int index;
    srand(time(NULL));
    
    while(true){

        if(mode == -1){
            queueNumber = QUEUE_A+rand() %3 ;//losowo wybrana kolejka
        }
        message m;
        m.letter1 = getRandomChar(0);
        m.letter2 = getRandomChar(0);
        m.letter3 = getRandomChar(0);
        m.priority = 1;

        // nieblokujace sprawdzenie
        if((index = insertMessage(queueNumber, m)) != -1){ //losowo wybrana kolejka
           // break;
            printf("full places in %s: %2d, inserting message: (%d %c%c%c)\n",queueName[queueNumber], index, m.priority, m.letter1, m.letter2, m.letter3);
           
        }

        

        sleep(3);
    }


    return 0;
}




