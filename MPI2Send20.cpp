#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI2Send20");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double number;
    if (rank == 0)
    {
        MPI_Status status;
        

        for (int i = 1; i < size; ++i) {
            MPI_Recv(&number, 1, MPI_DOUBLE, MPI_ANY_SOURCE, i, MPI_COMM_WORLD, &status);
            pt << number;
        }
    }
    else
    {
        int count;
        pt >> number;
        pt >> count;
        MPI_Send(&number, 1, MPI_DOUBLE, 0, count, MPI_COMM_WORLD);
    }
}
