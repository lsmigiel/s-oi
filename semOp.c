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

int semid;

int main(int ac, char *av[]){

    getSemaphores(&semid);

    if(ac == 1){
        printf("Specify opoeration!\n");
        return 0;
    }

    if(*av[1] == 'u'){
        printf("Sem up...\n");
        semUp(semid);
    }

    if(*av[1] == 'd'){
        printf("Sem down...\n");
        semDown(semid);
    }


    return 0;
}

