#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI3Coll10");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   

    double* arr = new double[size + 2];
    if (rank == 0)
    {
        for (int i = 0; i < size + 2; i++)
        {
            pt >> arr[i];
        }
    }

    auto count = new int[size];
    auto dis = new int[size];
    for (int i = 0; i < size; i++)
    {
        count[i] = 3;
        dis[i] = i;
    }

    auto rbuf = new double[3];

    MPI_Scatterv(arr, count, dis, MPI_DOUBLE, rbuf, 3, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < 3; i++)
    {
        pt << rbuf[i];
    }
}
