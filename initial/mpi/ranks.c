// The program can show ranks of MPI

#include <stdio.h>
#include <mpi.h>

int main (int argc, char* argv[])
{
    int errCode;

    if ((errCode = MPI_Init(&argc, &argv)) != 0)
    {
        return errCode;
    }

    int myRank;
    int mySize;

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &mySize);

    printf("My rank is: %d\n", myRank);
    printf("Size is: %d\n", mySize);

    MPI_Finalize();

    return 0;
}