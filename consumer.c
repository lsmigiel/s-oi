#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <zconf.h>
#include "data.h"


int shmid;

queue *shm, *s;

void getMemory();
int numberOfTakenEntries(int queueNumber);

int main()
{

    getMemory();
    s=shm;

    int i = 0;
    for(i=0; i<20; i++){
        printf("%d, %c%c%c\n", s[QUEUE_A].messages[i].priority, s[QUEUE_A].messages[i].letter1, s[QUEUE_A].messages[i].letter2, s[QUEUE_A].messages[i].letter3);
    }

    return 0;
}

void getMemory(){    

    if ((shmid = shmget(key, QUEUES_SIZE,  IPC_CREAT | 0777)) < 0) {
        perror("shmget");
       return;
    }

    shm = shmat(shmid, NULL, 0);

}

int numberOfTakenEntries(int queueNumber){   
    int i, j, takenEntries = 0;
    //wiadomosc z priority==-1 traktuje jak jej brak (UWAGA - brak wiadomosci to co innego niz pusta wiadomosc!);

    for(j=0; j<20; j++){
        if(shm[queueNumber].messages[j].priority != -1){
            takenEntries++;
        };
    }

    return takenEntries;
}