// min_x, min_t or start_x, start_t = 0 by condition

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <math.h>


// t = 0
double initial_function_for_x(int current_step, double delta_x) {
    double x = current_step * delta_x;
    return x;
}


// x = 0
double initial_function_for_t(int current_step, double delta_t) {
    double t = current_step * delta_t;
    return t;
}


double function_f(int k, int m) {
    return 0.1;
}


// 4-point scheme for u_{k+1} = u_top
double function_u_4(int k,
                    int m,
                    double delta_t,
                    double delta_x,
                    double u_left,
                    double u_middle,
                    double u_right) {
    return (function_f(k, m) +
            0.5 * delta_t * (u_right - 2 * u_middle + u_left) / pow(delta_x, 2) -
            (u_right - u_left) / (2 * delta_x)) * delta_t + u_middle;
}


// 3-point scheme (clear left corner scheme) for u_{k+1} = u_top
double function_u_3(int k,
                    int m,
                    double delta_t,
                    double delta_x,
                    double u_left,
                    double u_middle) {
    return (function_f(k, m) -
            (u_middle - u_left) / delta_x) * delta_t + u_middle;
}


int main(int argc, char* argv[]) {
    int i, j;

    // Conditions
    double max_t = 1, max_x = 1;

    int number_of_points_t, number_of_points_x;
    if (argc < 3) {
        number_of_points_t = 100;
        number_of_points_x = 100;
    } else {
        number_of_points_t = atoi(argv[1]);
        number_of_points_x = atoi(argv[2]);
    }

    double u[number_of_points_t][number_of_points_x];

    double delta_t = max_t / (number_of_points_t - 1);
    double delta_x = max_x / (number_of_points_x - 1);

    for (i = 0; i < number_of_points_x; i++) {
        u[0][i] = initial_function_for_x(i, delta_x);
    }
    for (i = 0; i < number_of_points_t; i++) {
        u[i][0] = initial_function_for_t(i, delta_t);
    }

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double time = MPI_Wtime();

    int diff = number_of_points_x / size;  // number of points per process
    int rest = number_of_points_x % size;

    int start, end;

    // Используется метод конвейерного параллелизма с геометрической баллансировкой
    // The method of pipelined parallelism with geometric balancing is used
    if (rank < rest) {
        diff++;
        start = rank * diff;
        end = start + diff - 1;
    } else {
        start = rank * diff + rest;
        end = start + diff - 1;
    }

    MPI_Request req1, req2, req3, req4, req5, req6, req7, req8;
    MPI_Status status;
    if (rank == 0) {
        if (size != 1) {
//            for (i = start; i < end; i++) {
//                u[0][i] = initial_function_for_x(i, delta_x);
//            }

            for (i = 1; i < number_of_points_t; i++) {
                // Get data from next rank
                MPI_Irecv(&u[i][end + 1], 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &req1);

                u[i][end] = function_u_4(
                        i,
                        end,
                        delta_t,
                        delta_x,
                        u[i - 1][end - 1],
                        u[i - 1][end],
                        u[i - 1][end + 1]);
                MPI_Isend(&u[i][end], 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &req2);

                for (j = start + 1; j <= end - 1; j++) {
                    u[i][j] = function_u_4(
                            i,
                            j,
                            delta_t,
                            delta_x,
                            u[i - 1][j - 1],
                            u[i - 1][j],
                            u[i - 1][j + 1]);
                }

                MPI_Wait(&req1, &status);
                MPI_Wait(&req2, &status);
            }
        } else {  // Consequent program
//            for (i = 0; i < number_of_points_x; i++) {
//                u[0][i] = initial_function_for_x(i, delta_x);
//            }

            for (i = 1; i < number_of_points_t; i++) {
                u[i][end] = function_u_3(
                        i,
                        end,
                        delta_t,
                        delta_x,
                        u[i - 1][end - 1],
                        u[i - 1][end]);
                for (j = 1; j <= end - 1; j++) {
                    u[i][j] = function_u_4(
                            i,
                            j,
                            delta_t,
                            delta_x,
                            u[i - 1][j - 1],
                            u[i - 1][j],
                            u[i - 1][j + 1]);
                }
            }
        }
    } else if (rank == size - 1) {
//        for (i = start; i < end; i++) {
//            u[0][i] = initial_function_for_x(i, delta_x);
//        }

        for (i = 1; i < number_of_points_t; i++) {
            MPI_Irecv(&u[i][start - 1], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &req3);

            u[i][start] = function_u_4(
                    i,
                    end,
                    delta_t,
                    delta_x,
                    u[i - 1][start - 1],
                    u[i - 1][start],
                    u[i - 1][start + 1]);
            MPI_Isend(&u[i][start], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &req4);

            u[i][end] = function_u_3(
                    i,
                    end,
                    delta_t,
                    delta_x,
                    u[i - 1][end - 1],
                    u[i - 1][end]);

            for (j = start + 1; j <= end - 1; j++) {
                u[i][j] = function_u_4(
                        i,
                        j,
                        delta_t,
                        delta_x,
                        u[i - 1][j - 1],
                        u[i - 1][j],
                        u[i - 1][j + 1]);
            }

            MPI_Wait(&req3, &status);
            MPI_Wait(&req4, &status);

        }
    } else {
//        for (i = start; i < end; i++) {
//            u[0][i] = initial_function_for_x(i, delta_x);
//        }

        for (i = 1; i < number_of_points_t; i++) {
            MPI_Irecv(&u[i][start - 1], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &req5);
            MPI_Irecv(&u[i][end + 1], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &req6);

            u[i][start] = function_u_4(
                    i,
                    end,
                    delta_t,
                    delta_x,
                    u[i - 1][start - 1],
                    u[i - 1][start],
                    u[i - 1][start + 1]);
            MPI_Isend(&u[i][start], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &req7);

            u[i][end] = function_u_4(
                    i,
                    end,
                    delta_t,
                    delta_x,
                    u[i - 1][end - 1],
                    u[i - 1][end],
                    u[i - 1][end + 1]);
            MPI_Isend(&u[i][end], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &req8);

            for (j = start + 1; j <= end - 1; j++) {
                u[i][j] = function_u_4(
                        i,
                        j,
                        delta_t,
                        delta_x,
                        u[i - 1][j - 1],
                        u[i - 1][j],
                        u[i - 1][j + 1]);
            }

            MPI_Wait(&req5, &status);
            MPI_Wait(&req6, &status);
            MPI_Wait(&req7, &status);
            MPI_Wait(&req8, &status);
        }
    }

    time = MPI_Wtime() - time;

    if (rank == 0) {
        for (j = end + 1; j < number_of_points_x; j++) {
            MPI_Recv(&u[number_of_points_t - 1][j], 1, MPI_DOUBLE, MPI_ANY_SOURCE, j, MPI_COMM_WORLD, &status);
        }

//        // Table output
//        printf("\nTable:\n");
//        for (j = 0; j < number_of_points_x; j++) {
//            printf("+-------");
//        }
//        printf("+\n");
//        for (i = number_of_points_t - 1; i >= 0; i--) {
//            printf("| ");
//            for (j = 0; j < number_of_points_x; j++) {
//                printf("%-5.2lf | ", u[i][j]);
//            }
//            printf("\n");
//            for (j = 0; j < number_of_points_x; j++) {
//                printf("+-------");
//            }
//            printf("+\n");
//        }

        // The latest row
        printf("\nThe latest row (x - u):\n");
        for (j = 0; j < number_of_points_x; j++) {
            printf("%15.12lf - %15.12lf\n", j * delta_x, u[number_of_points_t - 1][j]);
        }

        printf("\nTime: %f\nSize: %d\n\n", time, size);
    } else {
        for (j = start; j <= end; j++) {
            MPI_Send(&u[number_of_points_t - 1][j], 1, MPI_DOUBLE, 0, j, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
