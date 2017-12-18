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

int semMutexId;
int semEmptyId;
int semFullId;

queue *shm;


void initializeQueues(int semMutexId, queue *shm_param);


//running: ./producer [queueNumber]
int main(int argc, char *argv[])
{
    if(argc != 2){printf("Specify queue nnumber: 0, 1, 2 (ie A, B, C)\n"); return 0;}
    int queueNumber = atoi(argv[1]);

    getMemory(&shmid, &shm);
    getSemaphores(&semMutexId, mutexKey);
    getSemaphores(&semEmptyId, semEmptyKey);
    getSemaphores(&semFullId, semFullKey);
    
    semctl(semEmptyId, 0, SETVAL, 20);
    semctl(semFullId, 0, SETVAL, 0);
    semctl(semMutexId, 0, SETVAL, 1);

    initializeQueues(semMutexId, shm);

    int index;

    srand(time(NULL));

    while(true){

        message m;
        m.letter1 = getRandomChar(0);
        m.letter2 = getRandomChar(0);
        m.letter3 = getRandomChar(0);
        m.priority = 1;

        
        semDown(semEmptyId);
        semDown(semMutexId);
        // nieblokujace sprawdzenie
        if((index = insertMessage(QUEUE_A, shm, m)) == -1){ //liczba miejsc zapelnionych to u nas to samo co indeks gdzie wstawiamy message
           // break;
        }
        printf("full places in %s: %d, inserting another message...\n",queueName[queueNumber], index);
        semUp(semMutexId);
        semUp(semFullId);
        

        sleep(1);
    }

    removeMemory(); //czy tylko jeden proces moze zrobic removeMemory() czy wszystkie musza?
    removeSemaphores(mutexKey);
    

    return 0;
}



//inicjalizacja trzech kolejek "brakiem" wiadomosci
void initializeQueues(int semid, queue *shm_param){
    int i, j;

    printf("Initializing queues...\n");

    semDown(semid);

    //wiadomosc z priority==-1 traktuje jak jej brak (UWAGA - brak wiadomosci to co innego niz pusta wiadomosc!);
    for(i=0; i<3; i++){
        for(j=0; j<20; j++){
            shm_param[i].messages[j].priority = -1;
        }
    }
    semUp(semid);
}






