#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>


pthread_mutex_t mutex;

int size;

static int shared = 0;

typedef struct {
    int rank;
} thread_args;


void *thread_func(void *arg) {
    thread_args* arg_in = (thread_args*) arg;
    int rank = arg_in->rank;
    int local = 0;

    while (shared != rank);

    pthread_mutex_lock(&mutex);
    local = shared++;
    printf("Got: %d; rank: %d\n", local, rank);
    pthread_mutex_unlock(&mutex);

    return NULL;
}


int main(int argc, char *argv[]) {
    pthread_mutex_init(&mutex, NULL);

    int error;

    if (argc < 2) {
        printf("Usage: %s [<size>]\n", argv[0]);
    }
    size = (argc > 1) ? atoi(argv[1]) : 10;

    unsigned int total_time = - clock();

    pthread_t threads[size];
    thread_args threads_args[size];

    for (int i = 0; i < size; i++) {
        threads_args[i].rank = i;
        error = pthread_create(
                &threads[i],
                (pthread_attr_t *) NULL,
                thread_func,
                &threads_args[i]
                );
        if (error != 0) {
            printf("Error: cannot create thread %d, returned: %d\n", i, error);
            exit(-1);
        }
    }

    for (int i = 0; i < size; i++) {
        pthread_join(threads[i], (void **) NULL);
    }

    pthread_mutex_destroy(&mutex);

    total_time += clock();
    double search_time = (double) total_time / CLOCKS_PER_SEC;
    printf("Result = %d; time = %f\n", shared, search_time);

    return 0;
}