#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex;

typedef struct s_thread_args {
    char id;
    int work;
} thread_args;

// Name: Abby Stucki
// Date: 12/2/2024
// Description: Pthreads wait for semaphore then subtract from 'work' field
void* simple_pthread(void* args) {
    printf("\nThread started... id=%c, work=%d", ((struct s_thread_args*)args)->id, ((struct s_thread_args*)args)->work);
    while(((struct s_thread_args*)args)->work != 0) {
        sem_wait(&mutex);
        ((struct s_thread_args*)args)->work--;
        printf("\nid=%c, work=%d", ((struct s_thread_args*)args)->id, ((struct s_thread_args*)args)->work);
        sem_post(&mutex);
    }
    printf("\nThread ended... id=%c", ((struct s_thread_args*)args)->id);
}


// Name: Abby Stucki
// Date: 12/2/2024
// Description: 
int main() {
    srand(time(NULL));

    sem_init(&mutex, 0, 1);
    pthread_t tA, tB, tC, tD, tE;

    struct s_thread_args *tA_args;
    tA_args->id = 'A';
    tA_args->work = 10;
    struct s_thread_args *tB_args;
    tB_args->id = 'B';
    tB_args->work = 10;
    struct s_thread_args *tC_args;
    tC_args->id = 'C';
    tC_args->work = 10;
    struct s_thread_args *tD_args;
    tD_args->id = 'D';
    tD_args->work = 10;
    struct s_thread_args *tE_args;
    tE_args->id = 'E';
    tE_args->work = 10;

    pthread_create(&tA, NULL, simple_pthread, (void *)tA_args);
    pthread_create(&tB, NULL, simple_pthread, (void *)tB_args);
    pthread_create(&tC, NULL, simple_pthread, (void *)tC_args);
    pthread_create(&tD, NULL, simple_pthread, (void *)tD_args);
    pthread_create(&tE, NULL, simple_pthread, (void *)tE_args);

    pthread_join(tA, NULL);
    pthread_join(tB, NULL);
    pthread_join(tC, NULL);
    pthread_join(tD, NULL);
    pthread_join(tE, NULL);

    sem_destroy(&mutex);
    return 0;
}