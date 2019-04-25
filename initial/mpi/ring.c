// The program of transitions between MPI processes

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

    if (myRank == 0)
    {
        int transitions = 0;

        transitions++;
        MPI_Send(&transitions, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

        MPI_Recv(&transitions, 1, MPI_INT, mySize - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received information from process %d\n", 0, mySize - 1);

    }
    else
    {
        int transitions;
        MPI_Recv(&transitions, 1, MPI_INT, myRank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received information from process %d\n", myRank, myRank - 1);
        transitions++;
        if (myRank == mySize - 1)
        {
            MPI_Send(&transitions, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
        else
        {
            MPI_Send(&transitions, 1, MPI_INT, myRank + 1, 0, MPI_COMM_WORLD);
        }

    }

    MPI_Finalize();
    return 0;
}