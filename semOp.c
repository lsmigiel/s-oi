#include <sys/types.h>  //TODO remove unused libraries
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <stdbool.h>
#include <time.h>
#include "api.c"

int semMutexId;
int semEmptyId;
int semFullId;

//wywolanie: ./semOp down/up m/empty/full init?
int main(int ac, char *av[]){

    getSemaphores(&semMutexId, mutexKey);
    getSemaphores(&semEmptyId, semEmptyKey);

    getSemaphores(&semFullId, semFullKey);

    if(ac == 1){
        printf("Specify operation and semaphore!\n");
        return 0;
    }

    if(*av[1] == 'u'){
        if(*av[2] == 'm'){ //up on mutex
            printf("Sem mutex up...\n");
            semUp(semMutexId);
        }
        if(*av[2] == 'e'){ //up on empty
            printf("Sem empty up...\n");
            semUp(semEmptyId);
        }
        if(*av[2] == 'f'){ //up on full
            printf("Sem full up...\n");
            semUp(semFullId);
        }
    }

    if(*av[1] == 'd'){
        if(*av[2] == 'm'){//down on mutex
            printf("Sem mutex down...\n");
            semDown(semMutexId);
        }
        if(*av[2] == 'e'){ //down on empty
            printf("Sem empty down...\n");
            semDown(semEmptyId);
        }
        if(*av[2] == 'f'){ //down on full
            printf("Sem full down...\n");
            semDown(semFullId);
        }
    }


    return 0;
}

