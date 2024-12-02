#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <linux/random.h>

sem_t mutex;

typedef struct s_thread_args {
    char id;
    int work;
} thread_args;

// Name: Abby
// Date: 12/2/2024
// Description: Generate random number within range of 0 to max
static int get_rand(int max) {
    int i, rand_num;
    get_random_bytes(&i, sizeof(i));
    rand_num = i % max;
    return ((rand_num >= 0) ? rand_num : -rand_num);
}

// Name: Abby Stucki
// Date: 12/2/2024
// Description: Pthreads wait for semaphore then subtract from 'work' field
void* simple_pthread(thread_args args) {
    printf("\nThread started... id=%c, work=%d", args.id, args.work);
    while(args.work != 0) {
        sem_wait(&mutex);
        args.work--;
        printf("\nid=%c, work=%d", args.id, args.work);
        sem_post(&mutex);
    }
    printf("\nThread ended... id=%c", args.id);
}


// Name: Abby Stucki
// Date: 12/2/2024
// Description: 
int main() {
    sem_init(&mutex, 0, 1);
    pthread_t tA, tB, tC, tD, tE;

    thread_args tA_args = {'A', 10};
    thread_args tB_args = {'B', 10};
    thread_args tC_args = {'C', 10};
    thread_args tD_args = {'D', 10};
    thread_args tE_args = {'E', 10};

    pthread_create(&tA, NULL, simple_pthread, tA_args);
    pthread_create(&tB, NULL, simple_pthread, tB_args);
    pthread_create(&tC, NULL, simple_pthread, tC_args);
    pthread_create(&tD, NULL, simple_pthread, tD_args);
    pthread_create(&tE, NULL, simple_pthread, tE_args);

    pthread_join(tA, NULL);
    pthread_join(tB, NULL);
    pthread_join(tC, NULL);
    pthread_join(tD, NULL);
    pthread_join(tE, NULL);

    sem_destroy(&mutex);
    return 0;
}