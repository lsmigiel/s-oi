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

    int queueNumber;

    int index;

    srand(time(NULL));

    while(true){
        queueNumber = QUEUE_A+rand() %3 ;//losowo wybrana kolejka
        message m;
        m.letter1 = getRandomChar(0);
        m.letter2 = getRandomChar(0);
        m.letter3 = getRandomChar(0);
        m.priority = 2;

        // nieblokujace sprawdzenie
        if((index = insertMessage(queueNumber, m)) != -1){ //losowo wybrana kolejka
           // break;
            printf("full places in %s: %2d, inserting message: (%d %c%c%c)\n",queueName[queueNumber], index, m.priority, m.letter1, m.letter2, m.letter3);
           
        }

        

        sleep(3);
    }


    return 0;
}




