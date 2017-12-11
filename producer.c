#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <stdbool.h>
#include <time.h>
#include "data.h"


int maxQueueLength = 20;
int numberOfQueues = 3;

int shmid;


queue *shm, *s;

void getMemory();
void removeMemory();
void insertMessage(int queueNumber, int index, message m);
void initializeQueues();
int numberOfTakenEntries(int queueNumber);
char getRandomChar(int mode);
int getSemaphores();
void semDown(int num);
void semUp(int num);

int main()
{
    getMemory();
    initializeQueues();
    
    s = shm;

    srand(time(NULL));

    while(true){

        message m;
        m.letter1 = getRandomChar(0);
        m.letter2 = getRandomChar(0);
        m.letter3 = getRandomChar(0);
        m.priority = 0;

        int index = numberOfTakenEntries(QUEUE_A);

        // nieblokujace sprawdzenie
        if(index == 20){ //liczba miejsc zapelnionych to u nas to samo co indeks gdzie wstawiamy message
            //wait
        }
        
        
        printf("taken in QA: %d, inserting...\n", index);
        // semDown();
        insertMessage(QUEUE_A, index, m);
        // semUp();
        sleep(2);
    }
    printf("Done with qUEE A");


    removeMemory();
    return 0;
}

void getMemory(){
    

    if ((shmid = shmget(key, QUEUES_SIZE,  IPC_CREAT | 0777)) < 0) {
        perror("shmget");
       return;
    }

    shm = shmat(shmid, NULL, 0);

}


void removeMemory(){
    shmctl(key, 0, IPC_RMID);
}

void insertMessage(int queueNumber, int index, message m){

    shm[queueNumber].messages[index] = m;
}

void initializeQueues(){
    int i, j;
    //wiadomosc z priority==-1 traktuje jak jej brak (UWAGA - brak wiadomosci to co innego niz pusta wiadomosc!);
    for(i=0; i<3; i++){
        for(j=0; j<20; j++){
            shm[i].messages[j].priority = -1;
        }
    }
}

int numberOfTakenEntries(int queueNumber){   
    int i, j, takenEntries = 0;

    for(j=0; j<20; j++){
        if(shm[queueNumber].messages[j].priority != -1){
            takenEntries++;
        };
    }

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

int getSemaphores()
{
    // int semgetRet;
    // semgetRet = semget(IPC_PRIVATE, 3*numberOfQueues+1, 0600 |  SHM_W | SHM_R);
    // if(semgetRet <= 0)
	// {
	// 	printf("Error semget\n");
	// 	abort();
	// }else semaphoreKey=semgetRet;
	// for(unsigned int i=0;i<numberOfQueues;++i)
    // {
    //     semctl(semgetRet, MUTEX(i), SETVAL, 1);
    //     semctl(semgetRet, FULL(i), SETVAL, 0);
    //     semctl(semgetRet, EMPTY(i), SETVAL, maxQueueLength);
    // }
    // semctl(semgetRet, CONSOLERW, SETVAL, 1);
    // return semgetRet;
}


void semDown(int num)
{
    // struct sembuf sem = {0,0,0};
	// 	sem.sem_num = num;
	// 	sem.sem_op = -1;
	// 	semop(semaphoreKey, &sem, 1);
}

void semUp(int num)
{
    // struct sembuf sem = {0,0,0};
	// 	sem.sem_num = num;
	// 	sem.sem_op = 1;
	// 	semop(semaphoreKey, &sem, 1);
}

void removeSemaphores(int semKey)
{
    // semctl(semKey, 0, IPC_RMID);
}