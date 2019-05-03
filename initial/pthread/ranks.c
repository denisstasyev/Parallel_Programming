#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


int size;

typedef struct {
    int rank;
} thread_args;


void *thread_func(void *arg) {
    pthread_t this_thread;
    this_thread = pthread_self();

    thread_args* arg_in = (thread_args*) arg;

    printf("Thread: %lu; size: %d; rank: %d\n", this_thread, size, arg_in->rank + 1);

    return NULL;
}


int main(int argc, char *argv[]) {
    int error;

    if (argc != 2) {
        size = 4;
    } else {
        size = atoi(argv[1]);
    }

    pthread_t threads[size - 1];
    thread_args threads_args[size - 1];

    for (int i = 0; i < size - 1; i++) {
        threads_args[i].rank = i;
        error = pthread_create(
                &threads[i],
                (pthread_attr_t*) NULL,
                thread_func,
                &threads_args[i]
                );
        if (error != 0) {
            printf("Error: cannot create thread %d, returned: %d\n", i, error);
            exit(-1);
        }
    }

    pthread_t this_thread;
    this_thread = pthread_self();
    printf("Thread: %lu; size: %d; rank: 0\n", this_thread, size);

    for (int i = 0; i < size - 1; i++) {
        pthread_join(threads[i], (void**) NULL);
    }

    return 0;
}