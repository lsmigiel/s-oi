#define QUEUES_SIZE 480
#define QUEUE_A 0
#define QUEUE_B 1
#define QUEUE_C 2


typedef struct {
    int priority; //0 (niski),  1 (wysoki); Wartosc -1 oznacza brak wiadomosci;
    char letter1;
    char letter2;
    char letter3;     
} message;

typedef struct {
    message messages[20];
} queue;

queue queues[3]; //kolejka A, B lub C. sizeof(queues)==480

int shmKey = 89999; //shm

int mutexKey = 42; //mutex
int semFullKey = 43;
int semEmptyKey = 44;


char *queueName[3] = {"QUEUE_A", "QUEUE_B", "QUEUE_C"}; //tylko do printfa potrzebne