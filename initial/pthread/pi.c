#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int N;
double h;

int number_of_steps;
int number_of_cycles;

struct transfer_data {
    int index;
    double sum;
};


double y(double x) {
    return sqrt(4 - x * x);
}


void *mythread(void *input_data) {
    struct transfer_data* point = input_data;
    int index = point->index;

    int from_n, to_n;

    if (index == N - 1) {
        from_n = index * number_of_cycles;
        to_n = number_of_steps;
    } else {
        from_n = index * number_of_cycles;
        to_n = (index + 1) * number_of_cycles;
    }

    double from, to;
    double sum = 0;

    int i;
    for (i = from_n; i < to_n; i++) {
        from = i * h;
        to = (i + 1) * h;
        sum = sum + (y(from) + y(to)) * h / 2;
    }

    pthread_t mythid = pthread_self();
    printf("Thread = %lu, Calculation result = %lf\n", mythid, sum);
    point->sum = sum;
    return NULL;
}


int main() {
    printf("Enter N - number of threads: ");
    scanf("%d", &N);
    printf("Enter h - length of step: ");
    scanf("%lf", &h);

    number_of_steps = 2 / h; //diff
    number_of_cycles = number_of_steps / N;

    struct transfer_data data[N];

    pthread_t thid[N];

    int i;
    for (i = 0; i < N; i++) {
        data[i].index = i;
        int result = pthread_create(&thid[i], (pthread_attr_t *) NULL, mythread, &data[i]);
        if (result != 0) {
            printf("Error on thread create, return value = %d\n", result);
            exit(-1);
        }
    }


    for (i = 0; i < N; i++) {
        pthread_join(thid[i], (void **) NULL);
    }

    double result = 0;
    for (i = 0; i < N; i++) {
        result = result + data[i].sum;
    }

    pthread_t mythid = pthread_self();
    printf("Thread = %lu, Result pi: %lf\n", mythid, result);

    return 0;
}