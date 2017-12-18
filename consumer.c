#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <zconf.h>
#include "api.c"


int shmid;
int semid;

queue *shm;


int main()
{

    getMemory(&shmid, &shm);

    getSemaphores(&semid);

    int i = 0;
    for(i=0; i<20; i++){
        message m = shm[QUEUE_A].messages[i];
        printf("%d, %c%c%c\n", m.priority, m.letter1, m.letter2, m.letter3);
    }

    message m = getMessageFromQueue(QUEUE_A, shm, semid);
    printf("Removing message from QUEUE_A: %d, %c%c%c\n", m.priority, m.letter1, m.letter2, m.letter3);

    printf("After removing:\n");
    for(i=0; i<20; i++){
        message m = shm[QUEUE_A].messages[i];
        printf("%d, %c%c%c\n", m.priority, m.letter1, m.letter2, m.letter3);
    }


    // semDown(semid);
    // printf("Consumer w sekcji krytycznej1");

    // semUp(semid);

     
    return 0;
}

