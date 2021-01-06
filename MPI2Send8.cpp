#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI2Send8");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n;
    MPI_Status status;


    if (rank == 0) {
        MPI_Recv(&n, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        pt << n;
        pt << status.MPI_SOURCE;
    }
    else {
        
        pt >> n;
        if (n != 0)
            MPI_Send(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
}
