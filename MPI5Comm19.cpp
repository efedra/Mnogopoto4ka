#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI5Comm19");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);




    int N;
    pt >> N;


    int* dims = new int[3]{ 2, 2, size / 4 };

    int* periods = new int[3]{ 0, 0, 0 };

    MPI_Comm comm, comm_sub, comm_sub2;

    MPI_Cart_create(MPI_COMM_WORLD, 3, dims, periods, 0, &comm);

    int remain_dims[] = { 0, 1, 1 };

    MPI_Cart_sub(comm, remain_dims, &comm_sub);
    int remain_dims2[] = { 0, 1 };
    MPI_Cart_sub(comm_sub, remain_dims2, &comm_sub2);

    int* output = new int[size / 4];

    MPI_Gather(&N, 1, MPI_INT, output, 1, MPI_INT, 0, comm_sub2);

    int comm_rank;
    MPI_Comm_rank(comm_sub2, &comm_rank);

    if (comm_rank == 0)
        for (int i = 0; i < size / 4; ++i)
            pt << output[i];


}
