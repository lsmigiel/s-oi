#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <zconf.h>
#include "api.c"

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

    bool modifiedMode = false;
    int iterationsInModifiedMode = 0; //wieksze od 0 oznacza tryb zmodyfikowany

    while(true){

        message m = getMessageFromQueue(queueNumber);
        sleep(1);

        if(m.letter1 == '0' && m.letter2 == '0' && m.letter3  == '0'){ //jesli pusty kmunikat

            continue;
        }
        if(m.priority != 1 && iterationsInModifiedMode == 0){//niepusty komunikat ma zwykly priorytet lub najwyzszy
            char c = m.letter1; //odczytaj pierwsza litere
            if(c == 'X'){//nie wrzucaj komunikatu do żadnej kolejki i dalej go już nie przetwarzaj, przejdź do następnej iteracji
                continue;
            }
            m.letter1 = m.letter2;
            m.letter2 = m.letter3;
            m.letter3 = getRandomChar(1);
           
            int queueNumberToInsert;
            //Wrzuć zmodyfikowany komunikat do kolejki o identyfikatorze zgodnym z odczytanym znakiem
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

           insertMessage(queueNumberToInsert, m);

        }

        if(m.priority == 1){
            iterationsInModifiedMode += 10;
        }

        if(iterationsInModifiedMode > 0){
            // Jeśli (niepusty) komunikat ma priorytet wysoki, najpierw zmień tryb działania – przy
            // obsłudze tego i następnych 10 komunikatów pomijaj czynność opisaną powyżej
            // czcionką pochyłą. Jeśli proces działa już w trybie zmodyfikowanym, przedłuż działanie
            // tego trybu o kolejne 10 komunikatów. Zmiana dotyczy oczywiście tylko tego procesu.
            // Dalsza obsługa – jak w poprzednim kroku.
            
            char c = m.letter1; //odczytaj pierwsza litere
            if(c == 'X'){//nie wrzucaj komunikatu do żadnej kolejki i dalej go już nie przetwarzaj, przejdź do następnej iteracji
                continue;
            }
            m.letter1 = m.letter2;
            m.letter2 = m.letter3;
            m.letter3 = '0';
           
            int queueNumberToInsert;
            //Wrzuć zmodyfikowany komunikat do kolejki o identyfikatorze zgodnym z odczytanym znakiem
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

           insertMessage(queueNumberToInsert, m);


        }

        if(iterationsInModifiedMode > 0){
            iterationsInModifiedMode--;
        }

    }

    return 0;
}

