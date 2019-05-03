#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>


pthread_mutex_t mutex;

int size;
int diff;

int N;

static double result = 0;

typedef struct {
    int rank;
    double part_sum;
} thread_args;


void *thread_func(void *arg) {
    thread_args* arg_in = (thread_args*) arg;
    int rank = arg_in->rank;

    double sum = 0;
    int start, end;

    if (rank == 0) {
        start = 1;
        end = diff;
    } else if (rank == size - 1) {
        start = rank * diff + 1;
        end = N;
    } else {
        start = rank * diff + 1;
        end = (rank + 1) * diff;
    }

    for (int i = start; i <= end; i++) {
        sum += 1. / i;
    }

    arg_in->part_sum = sum;
//    printf("Sum: %f; rank: %d; start: %d / end: %d\n", sum, rank, start, end);

    pthread_mutex_lock(&mutex);
    result += sum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}


int main(int argc, char *argv[]) {
    pthread_mutex_init(&mutex, NULL);

    int error;

    if (argc < 3) {
        printf("Usage: %s [<size> <N>]\n", argv[0]);
    }
    size = (argc > 1) ? atoi(argv[1]) : 3;
    N = (argc > 2) ? atoi(argv[2]) : 1000;

    unsigned int total_time = - clock();

    diff = N / size;
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

//    double result = 0;
//    for (int i = 0; i < size; i++) {
//        result += threads_args[i].part_sum;
//    }

    total_time += clock();
    double search_time = (double) total_time / CLOCKS_PER_SEC;
    printf("Result: %.15f; time: %f\n", result, search_time);

    return 0;
}