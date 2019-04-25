#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <stdbool.h>
#include <time.h>


bool function_life(int rows, int columns, bool (*cells)[columns], int i, int j) {  // bool** cells
    if (i == 0) {  // lowest row
        if (j == 0) {  // first column
            if (cells[i][j] == 0) {
                return (cells[i + 1][j + 1] + cells[i][j + 1] + cells[i + 1][j] == 3) ? 1 : 0;
            } else {
                return ((cells[i + 1][j + 1] + cells[i][j + 1] + cells[i + 1][j] == 2) ||
                        (cells[i + 1][j + 1] + cells[i][j + 1] + cells[i + 1][j] == 3)) ? 1 : 0;
            }
        } else if (j == columns - 1) {  // last column
            if (cells[i][j] == 0) {
                return (cells[i + 1][j - 1] + cells[i][j - 1] + cells[i + 1][j] == 3) ? 1 : 0;
            } else {
                return ((cells[i + 1][j - 1] + cells[i][j - 1] + cells[i + 1][j] == 2) ||
                        (cells[i + 1][j - 1] + cells[i][j - 1] + cells[i + 1][j] == 3)) ? 1 : 0;
            }
        } else {  // middle columns
            if (cells[i][j] == 0) {
                return (cells[i][j - 1] + cells[i + 1][j - 1] + cells[i + 1][j] + cells[i + 1][j + 1] +
                        cells[i][j + 1] == 3) ? 1 : 0;
            } else {
                return ((cells[i][j - 1] + cells[i + 1][j - 1] + cells[i + 1][j] + cells[i + 1][j + 1] +
                         cells[i][j + 1] == 2) ||
                        (cells[i][j - 1] + cells[i + 1][j - 1] + cells[i + 1][j] + cells[i + 1][j + 1] +
                         cells[i][j + 1] == 3)) ? 1 : 0;
            }
        }
    }
    if (i == rows - 1) { // highest row
        if (j == 0) {  // first column
            if (cells[i][j] == 0) {
                return (cells[i - 1][j] + cells[i - 1][j + 1] + cells[i][j + 1] == 3) ? 1 : 0;
            } else {
                return ((cells[i - 1][j] + cells[i - 1][j + 1] + cells[i][j + 1] == 2) ||
                        (cells[i - 1][j] + cells[i - 1][j + 1] + cells[i][j + 1] == 3)) ? 1 : 0;
            }
        } else if (j == columns - 1) {  // last column
            if (cells[i][j] == 0) {
                return (cells[i - 1][j] + cells[i - 1][j - 1] + cells[i][j - 1] == 3) ? 1 : 0;
            } else {
                return ((cells[i - 1][j] + cells[i - 1][j - 1] + cells[i][j - 1] == 2) ||
                        (cells[i - 1][j] + cells[i - 1][j - 1] + cells[i][j - 1] == 3)) ? 1 : 0;
            }
        } else {  // middle columns
            if (cells[i][j] == 0) {
                return (cells[i][j - 1] + cells[i - 1][j - 1] + cells[i - 1][j] + cells[i - 1][j + 1] +
                        cells[i][j + 1] == 3) ? 1 : 0;
            } else {
                return ((cells[i][j - 1] + cells[i - 1][j - 1] + cells[i - 1][j] + cells[i - 1][j + 1] +
                         cells[i][j + 1] == 2) ||
                        (cells[i][j - 1] + cells[i - 1][j - 1] + cells[i - 1][j] + cells[i - 1][j + 1] +
                         cells[i][j + 1] == 3)) ? 1 : 0;
            }
        }
    } else { // middle rows
        if (j == 0) {  // first column
            if (cells[i][j] == 0) {
                return (cells[i - 1][j] + cells[i - 1][j + 1] + cells[i][j + 1] + cells[i + 1][j + 1] +
                        cells[i + 1][j] == 3) ? 1 : 0;
            } else {
                return ((cells[i - 1][j] + cells[i - 1][j + 1] + cells[i][j + 1] + cells[i + 1][j + 1] +
                         cells[i + 1][j] == 2) ||
                        (cells[i - 1][j] + cells[i - 1][j + 1] + cells[i][j + 1] + cells[i + 1][j + 1] +
                         cells[i + 1][j] == 3)) ? 1 : 0;
            }
        } else if (j == columns - 1) {  // last column
            if (cells[i][j] == 0) {
                return (cells[i - 1][j] + cells[i - 1][j - 1] + cells[i][j - 1] + cells[i + 1][j - 1] +
                        cells[i + 1][j] == 3) ? 1 : 0;
            } else {
                return ((cells[i - 1][j] + cells[i - 1][j - 1] + cells[i][j - 1] + cells[i + 1][j - 1] +
                         cells[i + 1][j] == 2) ||
                        (cells[i - 1][j] + cells[i - 1][j - 1] + cells[i][j - 1] + cells[i + 1][j - 1] +
                         cells[i + 1][j] == 3)) ? 1 : 0;
            }
        } else {  // middle columns
            if (cells[i][j] == 0) {
                return (cells[i + 1][j - 1] + cells[i + 1][j] + cells[i + 1][j + 1] + cells[i][j - 1] +
                        cells[i - 1][j - 1] + cells[i - 1][j] + cells[i - 1][j + 1] + cells[i][j + 1] == 3) ? 1 : 0;
            } else {
                return ((cells[i + 1][j - 1] + cells[i + 1][j] + cells[i + 1][j + 1] + cells[i][j - 1] +
                         cells[i - 1][j - 1] + cells[i - 1][j] + cells[i - 1][j + 1] + cells[i][j + 1] == 2) ||
                        (cells[i + 1][j - 1] + cells[i + 1][j] + cells[i + 1][j + 1] + cells[i][j - 1] +
                         cells[i - 1][j - 1] + cells[i - 1][j] + cells[i - 1][j + 1] + cells[i][j + 1] == 3)) ? 1 : 0;
            }
        }
    }
}


int main(int argc, char *argv[]) {
    int i, j;

    if (argc < 4) {
        printf("Usage: %s [<rows> <columns> <ticks>]\n", argv[0]);
    }
    int rows = (argc > 1) ? atoi(argv[1]) : 100;
    int columns = (argc > 2) ? atoi(argv[2]) : 100;
    int ticks = (argc > 3) ? atoi(argv[3]) : 10;

    if (rows < 2 || columns < 2) {
        printf("<rows> and <columns> values must be >= 2.\n");
        exit(1);
    }

    bool cells[rows][columns];

    srand(time(NULL));
    for (i = 0; i < rows; i++) {
        for (j = 0; j < columns; j++) {
            cells[i][j] = rand() % 2;
        }
    }

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("\nInitial data:\n\n");
        for (i = 0; i < rows; i++) {
            for (j = 0; j < columns; j++) {
                printf(" %d ", cells[i][j]);
            }
            printf("\n");
        }
    }

    int diff = columns / size;  // number of points per process
    int rest = columns % size;

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

    MPI_Status status;
    MPI_Request req1, req2, req3, req4;

    MPI_Datatype parttype;
    MPI_Type_contiguous(rows, MPI_C_BOOL, &parttype);
    MPI_Type_commit(&parttype);

    double time = -MPI_Wtime();

    bool cells_updated[rows][columns];
    int k;
    if (size > 1) {
        for (k = 0; k < ticks; k++) {
            if (rank == 0) {
                if (k > 0) {
                    bool data_recv[rows];
                    MPI_Recv(data_recv, 1, parttype, rank + 1, 0, MPI_COMM_WORLD, &status);
                    for (i = 0; i < rows; i++) {
                        cells[i][end] = data_recv[i];
                    }
                }

                bool data_send[rows];
                for (i = 0; i < rows; i++) {
                    cells_updated[i][end - 1] = function_life(rows, columns, cells, i, end - 1);
                    data_send[i] = cells_updated[i][end - 1];
                }
                MPI_Isend(data_send, 1, parttype, rank + 1, 0, MPI_COMM_WORLD, &req1);

                for (i = 0; i < rows; i++) {
                    for (j = start; j < end - 1; j++) {
                        cells_updated[i][j] = function_life(rows, columns, cells, i, j);
                    }
                }

                MPI_Wait(&req1, &status);

            } else if (rank == size - 1) {
                bool data_recv[rows];
                MPI_Recv(data_recv, 1, parttype, rank - 1, 0, MPI_COMM_WORLD, &status);
                for (i = 0; i < rows; i++) {
                    cells[i][start - 1] = data_recv[i];
                }

                if (k < ticks - 1) {
                    bool data_send[rows];
                    for (i = 0; i < rows; i++) {
                        cells_updated[i][start] = function_life(rows, columns, cells, i, start);
                        data_send[i] = cells_updated[i][start];
                    }
                    MPI_Isend(data_send, 1, parttype, rank - 1, 0, MPI_COMM_WORLD, &req2);
                }

                for (i = 0; i < rows; i++) {
                    for (j = start + 1; j < end; j++) {
                        cells_updated[i][j] = function_life(rows, columns, cells, i, j);
                    }
                }

                if (k < ticks - 1) {
                    MPI_Wait(&req2, &status);
                }

            } else {
                bool data_recv[rows];
                MPI_Recv(data_recv, 1, parttype, rank - 1, 0, MPI_COMM_WORLD, &status);
                for (i = 0; i < rows; i++) {
                    cells[i][start - 1] = data_recv[i];
                }

                if (k > 0) {
                    bool data_recv[rows];
                    MPI_Recv(data_recv, 1, parttype, rank + 1, 0, MPI_COMM_WORLD, &status);
                    for (i = 0; i < rows; i++) {
                        cells[i][end] = data_recv[i];
                    }
                }

                if (k < ticks - 1) {
                    bool data_send[rows];
                    for (i = 0; i < rows; i++) {
                        cells_updated[i][start] = function_life(rows, columns, cells, i, start);
                        data_send[i] = cells_updated[i][start];
                    }
                    MPI_Isend(data_send, 1, parttype, rank - 1, 0, MPI_COMM_WORLD, &req3);
                }


                bool data_send[rows];
                for (i = 0; i < rows; i++) {
                    cells_updated[i][end - 1] = function_life(rows, columns, cells, i, end - 1);
                    data_send[i] = cells_updated[i][end - 1];
                }
                MPI_Isend(data_send, 1, parttype, rank + 1, 0, MPI_COMM_WORLD, &req4);

                for (i = 0; i < rows; i++) {
                    for (j = start; j < end - 1; j++) {
                        cells_updated[i][j] = function_life(rows, columns, cells, i, j);
                    }
                }

                if (k < ticks - 1) {
                    MPI_Wait(&req3, &status);
                }
                MPI_Wait(&req4, &status);
            }


            for (i = 0; i < rows; i++) {
                for (j = 0; j < columns; j++) {
                    cells[i][j] = cells_updated[i][j];
                }
            }
        }
    } else {  // Consequent program
        for (k = 0; k < ticks; k++) {
            for (i = 0; i < rows; i++) {
                for (j = 0; j < columns; j++) {
                    cells_updated[i][j] = function_life(rows, columns, cells, i, j);
                }
            }


            for (i = 0; i < rows; i++) {
                for (j = 0; j < columns; j++) {
                    cells[i][j] = cells_updated[i][j];
                }
            }
        }
    }

    time += MPI_Wtime();

    if (rank == 0) {
        for (j = end + 1; j < columns; j++) {
            bool data_recv[rows];
            MPI_Recv(data_recv, 1, parttype, MPI_ANY_SOURCE, j, MPI_COMM_WORLD, &status);
            for (i = 0; i < rows; i++) {
                cells[i][j] = data_recv[i];
            }
        }

        printf("\nResult data:\n\n");
        for (i = 0; i < rows; i++) {
            for (j = 0; j < columns; j++) {
//                if (cells[i][j] > 1) cells[i][j] = 1;
                printf(" %d ", cells[i][j]);  // there is a bug with sending MPI_C_BOOL type with MPI_Type_contiguous(...)
            }
            printf("\n");
        }

        printf("\nTime: %f\nTicks: %d\n\n", time, ticks);
    }
    else {
        bool data_send[rows];
        for (j = start; j <= end; j++) {
            for (i = 0; i < rows; i++) {
                data_send[i] = cells_updated[i][j];
            }
            MPI_Send(data_send, 1, parttype, 0, j, MPI_COMM_WORLD);
        }
    }

    MPI_Type_free(&parttype);
    MPI_Finalize();
    return 0;
}