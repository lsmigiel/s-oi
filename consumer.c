#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <zconf.h>
#include "api.c"


int shmid;

int semMutexId;
int semEmptyId;
int semFullId;

queue *shm;


int main()
{

    getMemory(&shmid, &shm);

    getSemaphores(&semMutexId, mutexKey);
    getSemaphores(&semEmptyId, semEmptyKey);
    getSemaphores(&semFullId, semFullKey);


    int i = 0;
    while(true){
        semDown(semFullId);
        semDown(semMutexId);

        message m = getMessageFromQueue(QUEUE_A, shm);
        printf("Removing message from QUEUE_A: %d, %c%c%c\n", m.priority, m.letter1, m.letter2, m.letter3);

        // printf("After removing:\n");
        // for(i=0; i<20; i++){
        //     message m = shm[QUEUE_A].messages[i];
        //     printf("%d, %c%c%c\n", m.priority, m.letter1, m.letter2, m.letter3);
        // }

        semUp(semMutexId);
        semUp(semEmptyId);

        if(m.letter1 == '0' && m.letter2 == '0' && m.letter3  == '0'){ //jesli pusty kmunikat
            continue;
        }
        if(m.priority == 0){
            char c = m.letter1; //odczytaj pierwsza litere
            if(c == 'X'){//nie wrzucaj komunikatu do żadnej kolejki i dalej go już nie przetwarzaj, przejdź do następnej iteracji
                continue;
            }
            m.letter1 = m.letter2;
            m.letter2 = m.letter3;
            m.letter3 = getRandomChar(1);
            int queueNumber;
            // if(c == 'A'){
            //     queueNumber = QUEUE_A;
            // }
            // else if(c == 'B'){
            //     queueNumber = QUEUE_B;
            // }
            // else if(c == 'C'){
            //     queueNumber = QUEUE_C;
            // }
            // insertMessage(queueNumber, shm, m);
        }



        sleep(1);
    }

    return 0;
}

