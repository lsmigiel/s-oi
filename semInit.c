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

//ID kolejnych trojek semaforow (po 1 dla kazdej kolejki z kazdego rodzaju)
int semMutexId; 
int semEmptyId;
int semFullId;

queue *shm;


void initializeQueues(int semMutexId, queue *shm_param);


int main()
{
    getMemory(&shm);
    getSemaphores(&semMutexId, mutexKey);    
    getSemaphores(&semEmptyId, semEmptyKey);
    getSemaphores(&semFullId, semFullKey);
    
    //for every queue initialize its semaphores
    semctl(semEmptyId, 0, SETVAL, 20);
    semctl(semFullId, 0, SETVAL, 0);
    semctl(semMutexId, 0, SETVAL, 1);

    semctl(semEmptyId, 1, SETVAL, 20);
    semctl(semFullId, 1, SETVAL, 0);
    semctl(semMutexId, 1, SETVAL, 1);

    semctl(semEmptyId,2, SETVAL, 20);
    semctl(semFullId, 2, SETVAL, 0);
    semctl(semMutexId, 2, SETVAL, 1);

   initializeQueues(semMutexId, shm);


    return 0;
}




//inicjalizacja trzech kolejek "brakiem" wiadomosci
void initializeQueues(int semid, queue *shm_param){
    int i, j;

    printf("Initializing queues...\n");
    //wiadomosc z priority==-1 traktuje jak jej brak (UWAGA - brak wiadomosci to co innego niz pusta wiadomosc!);
    for(i=0; i<3; i++){
        semDown(semid, i);
        for(j=0; j<20; j++){
            shm_param[i].messages[j].priority = -1;
        }
        semUp(semid, i);
    }

}




