
/* The program of calculation integral from 0.01 to 10 of function cos(1/x) over dx */

// Use "gcc -o 1.out lab2.c -lpthread -lm" to compile
// and "./1.out" to run.

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>


pthread_mutex_t mutex;
double interval_start = 0.01, interval_end = 10;

int size;
int diff, rest;

int points;

static double result = 0;

// k - number of points where function cos(1/x) is equal to 0
#define count_zero_points (int)(1 / (M_PI * interval_start) - 1 / 2)

int count_zero_per_one;  // per each thread the same number of zero points

typedef struct {
    int rank;
} thread_args;


double get_coord(int rank) {
    return 2 / M_PI / (2 * count_zero_per_one * (size - rank - 1) + 1);
}

double func_integral(double coord_left, double coord_step) {
    return coord_step * (cos(1 / coord_left) + cos(1 / (coord_left + coord_step))) / 2;
}


void *thread_func(void *arg) {
    thread_args* arg_in = (thread_args*) arg;

    int rank = arg_in->rank;

    unsigned int t_time = - clock();

    int start, end;
    if (rank < rest) {
        start = rank * (diff + 1);
        end = start + diff;
    } else {
        start = rank * diff + rest;
        end = start + diff - 1;
    }

    double coord_start, coord_end;
    if (rank == 0) {
        coord_start = interval_start;
        if (size == 1) {
            coord_end = interval_end;
        } else {
            coord_end = get_coord(rank);
        }
    } else if (rank == size - 1) {
        coord_start = get_coord(rank - 1);
        coord_end = interval_end;
    } else {
        coord_start = get_coord(rank - 1);
        coord_end = get_coord(rank);
    }

    double step;
    step = (coord_end - coord_start) / (end - start + 1);

    double sum = 0;
    double coord = coord_start;
    for (int i = start; i <= end; i++) {
        sum += func_integral(coord, step);
        coord += step;
    }

    t_time += clock();
    double s_time = (double) t_time / CLOCKS_PER_SEC;

    printf("Sum: %.15f; rank: %d; time: %f; start: %.15f / end: %.15f; step: %.15f\n", sum, rank, s_time, coord_start, coord_end, step);

    pthread_mutex_lock(&mutex);
    result += sum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}


int main(int argc, char *argv[]) {
    pthread_mutex_init(&mutex, NULL);

    int error;

    if (argc < 3) {
        printf("Usage: %s [<size> <points>]\n", argv[0]);
    }
    size = (argc > 1) ? atoi(argv[1]) : 10;
    points = (argc > 2) ? atoi(argv[2]) : 100000;

    unsigned int total_time = - clock();

    diff = points / size;
    rest = points % size;

    count_zero_per_one = count_zero_points / size;

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
    printf("Result: %.15f; time: %f\n", result, search_time);

    return 0;
}