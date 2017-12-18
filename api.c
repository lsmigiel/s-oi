#include <sys/types.h>  //TODO remove unused libraries
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <stdbool.h>
#include <time.h>
#include "data.h"
  



void getMemory(int *shmid_param, queue** shm_param){ 

    if ((*shmid_param = shmget(shmKey, QUEUES_SIZE,  IPC_CREAT | 0777)) < 0) {
        perror("shmget");
       return;
    }

    *shm_param = shmat(*shmid_param, NULL, 0);

}


int getSemaphores(int* semid, int semKey)
{
    /*
     * If IPC_CREAT is used alone, semget() either returns the semaphore set identifier for a newly created set,
     * or returns the identifier for a set which exists with the same key value.
     */
    if((*semid = semget( semKey, 3, IPC_CREAT | 0777 )) == -1) //tworzymy 3 mutexy, 3 fulle lub 3 empty
    {
        return(-1); 
    }
}


void removeMemory(){
    shmctl(shmKey, 0, IPC_RMID);
}

void removeSemaphores(int semKey)
{
     semctl(semKey, 0, IPC_RMID);
}

void semDown(int semid_param, int queueNumber){

    struct sembuf sem;
    int semopRet;

    /* Which semaphore in the semaphore array : */
    sem.sem_num = queueNumber;
    /* Which operation? Add -1 to semaphore value : */
    sem.sem_op = -1;
    /* Set the flag so we will wait : */
    sem.sem_flg = 0;

    /* So do the operation! */
    semopRet = semop(semid_param, &sem, 1);

    if(semopRet == 0) {
        // printf("Successful Down by program.\n");
    }
    else {
         printf("Down operation did not succeed!!!\n");
    }
}

void semUp(int semid_param, int queueNumber){

    struct sembuf sem;
    int semopRet;
    
    /* Which semaphore in the semaphore array : */
    sem.sem_num = queueNumber;
    /* Which operation? Add 1 to semaphore value : */
    sem.sem_op = 1;
    /* Set the flag so we will wait : */
    sem.sem_flg = 0;

    /* So do the operation! */
    semopRet = semop(semid_param, &sem, 1);

    if(semopRet == 0) {
        // printf("Successful Up operation.\n");
    }
    else {
        printf("Up peration did not succeed!!!!\n");
    }

}


/*
return index of newly inserted messages. If failed, returns -1;
*/
int insertMessage(int queueNumber, queue* shm_param, message m){
 ///polaczyc moment sprawdzenia wolnego miejsca i ewentualnego wstawienia wiadomosci
    // semDown(semid);

    int i, takenEntries = 0;

    for(i=0; i<20; i++){
        if(shm_param[queueNumber].messages[i].priority != -1){
            takenEntries++;
        };
    }


    if(takenEntries >= 20 ){
        printf("Queue %d is full", queueNumber);
        return -1; //TODO should've wait on a semafore until consumer increases its value...
    }

    shm_param[queueNumber].messages[takenEntries] = m;

    // semUp(semid);

    return takenEntries;
}


char getRandomChar(int mode){
    int r;
    if(mode==0) r = rand()%3; //choose A, B or C
    else if(mode==1) r = rand()%4; //A, B, C, or X

    if(r==0) return 'A';
    if(r==1) return 'B';
    if(r==2) return 'C';
    if(r==3) return 'X';
}


//get and remove message at index 0 from queue 
message getMessageFromQueue(int queueNumber, queue* shm_param){
    // semDown(semid);

    int i=0, takenEntries = 0;
    // int normalPriorityPresent;
    // int 
    //if takenEntries > 0....

    int indexToTakeMessage = 0;
    for(i = 0; i<=19; i++){
        if(shm_param[queueNumber].messages[i].priority == 1){
            indexToTakeMessage = i;
            break;
        }
    }
    message m = shm_param[queueNumber].messages[indexToTakeMessage];
    printf("Removing message on index %d from %s: (%d %c%c%c)\n",indexToTakeMessage,queueName[queueNumber], m.priority, m.letter1, m.letter2, m.letter3);


    //moving other messages to the front of the queue:
    i = indexToTakeMessage;
    while(i<19 && shm_param[queueNumber].messages[i+1].priority != -1){
        shm_param[queueNumber].messages[i] = shm_param[queueNumber].messages[i+1]; 
        shm_param[queueNumber].messages[i+1].priority = -1; //the last is empty
        i++;
    }


    // semUp(semid);

    return m;
}