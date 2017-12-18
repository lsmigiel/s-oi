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

int shmid;

//ID kolejnych trojek semaforow (po 1 dla kazdej kolejki z kazdego rodzaju)
int semMutexId; 
int semEmptyId;
int semFullId;

queue *shm;



//running: ./producer queueNumber
int main(int argc, char *argv[])
{

    int queueNumber;

    getMemory(&shmid, &shm);
    getSemaphores(&semMutexId, mutexKey);
    getSemaphores(&semEmptyId, semEmptyKey);
    getSemaphores(&semFullId, semFullKey);

    int index;

    srand(time(NULL));

    while(true){
        queueNumber = QUEUE_A+rand() %3 ;//losowo wybrana kolejka
        message m;
        m.letter1 = getRandomChar(0);
        m.letter2 = getRandomChar(0);
        m.letter3 = getRandomChar(0);
        m.priority = 1;
        semDown(semEmptyId, queueNumber);
        semDown(semMutexId, queueNumber);
        // nieblokujace sprawdzenie
        if((index = insertMessage(queueNumber, shm, m)) == -1){ //losowo wybrana kolejka
           // break;
        }
        printf("full places in %s: %2d, inserting message: (%d %c%c%c)\n",queueName[queueNumber], index, m.priority, m.letter1, m.letter2, m.letter3);
        semUp(semMutexId, queueNumber);
        semUp(semFullId, queueNumber);
        

        sleep(3);
    }


    return 0;
}




