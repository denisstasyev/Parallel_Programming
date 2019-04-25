// The program of calculation sum of the row 1 / n

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
        int n;
        printf("Enter n: \n");
        scanf("%d", &n);
        double timeStart = MPI_Wtime();

        int i;
        for(i = 1; i < mySize; i++)
        {
            MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        double sum = 0;
        i = 1;
        while (myRank + mySize * i <= n)
        {
            sum += 1. / (myRank + mySize * i);
            i++;
        }

        for(i = 1; i < mySize; i++)
        {
            double s;
            MPI_Recv(&s, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += s;
        }

        double timeEnd = MPI_Wtime();
        printf("Result sum is %f; time: %f\n", sum, timeEnd - timeStart);
    }
    else
    {
        int n;
        MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int i = 0;
        double sum = 0;
        while (myRank + mySize * i <= n)
        {
            sum += 1. / (myRank + mySize * i);
            i++;
        }

        MPI_Send(&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}