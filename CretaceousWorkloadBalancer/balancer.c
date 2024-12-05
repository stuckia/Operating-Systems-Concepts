#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex;
sem_t semaphores[3];

typedef struct {
    char id;
    int work;
} s_thread_args;

// Name: Abby Stucki
// Date: 12/3/2024
// Description: Return a random integer (inclusive) between lower and upper bound
int get_rand_num(int lower_bound, int upper_bound) {
    return rand() % (upper_bound - lower_bound + 1) + lower_bound;
} 

// Name: Abby Stucki
// Date: 12/3/2024
// Description: Sleep for a random amount of milliseconds (0-10ms)
void rand_sleep(int milliseconds) {
    struct timespec rem, req = {0, (rand() % (milliseconds + 1)) * 1000000};
    nanosleep(&rem, &req);
}

// Name: Abby Stucki
// Date: 12/2/2024
// Description: Pthreads wait for semaphore then subtract from 'work' field
void* simple_pthread(void* args) {
    s_thread_args *current_args = (s_thread_args*)args;
    printf("Thread started... id=%c, work=%d\n", current_args->id, current_args->work);
    while(current_args->work > 0) {
        sem_wait(&mutex);
        current_args->work--;
        printf("id=%c, work=%d\n", current_args->id, current_args->work);
        sem_post(&mutex);
        sleep(1);
    }
    printf("Thread ended... id=%c\n", current_args->id);
    pthread_exit(NULL);
}

// Name: Abby Stucki
// Date: 12/2/2024
// Description:
void* complex_pthread(void* args) {
    s_thread_args *current_args = (s_thread_args*)args;
    
    printf("Thread started... id=%c, work=%d\n", current_args->id, current_args->work);
    while(current_args->work > 0) {
        int num_of_sems = get_rand_num(1,3);
        int req_sems[3];
        for(int i=0; i<3; i++) {
            req_sems[i] = 0;
        }

        int req = 0;
        while(req < num_of_sems) {
            int rand_sem = get_rand_num(0,2);
            if(req_sems[rand_sem]) {
                continue;
            }
            req_sems[rand_sem] = 1;
            req++;
        }

        int held_sems[3] = {0,0,0};
        int held = 0;

        while (held < num_of_sems)
        {
            // try to grab required number of semaphores
            for(int i=0; i<3; i++) {
                if(req_sems[i] && sem_trywait(&semaphores[i])==0) {
                    held_sems[i] = 1;
                    held++;
                    printf("%c>%d ", current_args->id, i);
                }
            }

            // if not enough semaphores obtained, post what is held then sleep
            if(held != num_of_sems) {
                for(int i=0; i<3; i++) {
                    if(held_sems[i]) {
                        printf("\n");
                        held_sems[i] = 0;
                        sem_post(&semaphores[i]);
                        printf("%c<%d ", current_args->id, i);
                    }
                }
                held=0;
                rand_sleep(10);
            }
        }

        // critical section
        printf("\n%c has %d left\n", current_args->id, current_args->work);
        current_args->work--;
        held=0;

        // post held semaphores
        for(int i=0; i<3; i++) {
            if(held_sems[i]) {
                printf("\n");
                held_sems[i] = 0;
                sem_post(&semaphores[i]);
                printf("%c<%d ", current_args->id, i);
            }
        }
        rand_sleep(10);
    }

    pthread_exit(NULL);
}

// Name: Abby Stucki
// Date: 12/2/2024
// Description: 
int main(int argc, char *argv[]) {
    srand(time(NULL));

    // initialize pid dictionary
    char pid_dict[5] = {'A', 'B', 'C', 'D', 'E'};

    // PHASE 1
    printf("Beginning phase 1:\n");
    pthread_t threads[5];
    s_thread_args thread_args[5];
    sem_init(&mutex, 0, 1);

    // reset threads
    for(int i=0; i<5; i++) {
        thread_args[i].id = pid_dict[i];
        thread_args[i].work = 10;
        pthread_create(&threads[i], NULL, simple_pthread, &thread_args[i]);
    }

    for(int i=0; i<5; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&mutex);

    // PHASE 2
    printf("\nBeginning phase 2:\n");
    for(int i=0; i<3; i++) {
        sem_init(&semaphores[i], 0, 1);
    }

    // reset threads
    for(int i=0; i<5; i++) {
        thread_args[i].id = pid_dict[i];
        thread_args[i].work = 10;
        pthread_create(&threads[i], NULL, complex_pthread, &thread_args[i]);
    }

    for(int i=0; i<5; i++) {
        pthread_join(threads[i], NULL);
    }

    for(int i=0; i<3; i++) {
        sem_destroy(&semaphores[i]);
    }

    return 0;
}