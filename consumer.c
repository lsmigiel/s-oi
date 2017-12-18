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

//running: ./consumer queueNumber
/**
 * removes messages from queue specified in parameter
 * inserts modified messages to queue selected by m.letter1
 * 
 */
int main(int argc, char *argv[])
{
    if(argc != 2){printf("Specify queue nnumber: 0, 1, 2 (ie A, B, C)\n"); return 0;}
    int queueNumber = atoi(argv[1]);

    getMemory(&shmid, &shm);
    getSemaphores(&semMutexId, mutexKey);
    getSemaphores(&semEmptyId, semEmptyKey);
    getSemaphores(&semFullId, semFullKey);


   // int i = 0;
    while(true){

        semDown(semFullId, queueNumber);
        semDown(semMutexId, queueNumber);
        message m = getMessageFromQueue(queueNumber, shm);


        // printf("After removing:\n");
        // for(i=0; i<20; i++){
        //     message m = shm[QUEUE_A].messages[i];
        //     printf("%d, %c%c%c\n", m.priority, m.letter1, m.letter2, m.letter3);
        // }

        semUp(semMutexId, queueNumber);
        semUp(semEmptyId, queueNumber);

        if(m.letter1 == '0' && m.letter2 == '0' && m.letter3  == '0'){ //jesli pusty kmunikat
            continue;
        }
        if(m.priority == 0){//niepusty komunikat ma zwykly priorytet
            char c = m.letter1; //odczytaj pierwsza litere
            if(c == 'X'){//nie wrzucaj komunikatu do żadnej kolejki i dalej go już nie przetwarzaj, przejdź do następnej iteracji
                continue;
            }
            m.letter1 = m.letter2;
            m.letter2 = m.letter3;
            m.letter3 = getRandomChar(1);
            int queueNumberToInsert;
            if(c == 'A'){
                queueNumberToInsert = QUEUE_A;
            }
            else if(c == 'B'){
                queueNumberToInsert = QUEUE_B;
            }
            else if(c == 'C'){
                queueNumberToInsert = QUEUE_C;
            }

            printf("Prosument inserts (%d %c%c%c) to %s...\n", m.priority,m.letter1,m.letter2, m.letter3, queueName[queueNumberToInsert]);
            semDown(semEmptyId, queueNumberToInsert);
            semDown(semMutexId, queueNumberToInsert);
            insertMessage(queueNumberToInsert, shm, m);
            semUp(semMutexId, queueNumberToInsert);
            semUp(semFullId, queueNumberToInsert);
        }
        if(m.priority == 1){
            // Jeśli (niepusty) komunikat ma priorytet wysoki, najpierw zmień tryb działania – przy
            // obsłudze tego i następnych 10 komunikatów pomijaj czynność opisaną powyżej
            // czcionką pochyłą. Jeśli proces działa już w trybie zmodyfikowanym, przedłuż działanie
            // tego trybu o kolejne 10 komunikatów. Zmiana dotyczy oczywiście tylko tego procesu.
            // Dalsza obsługa – jak w poprzednim kroku.
        }



        sleep(1);
    }

    return 0;
}

