#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI5Comm16");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);



    double N;
    if  (rank == 0 || rank == 1) {
             pt >> N;
    }




    int* dims = new int[2]{ size / 2, 2 };
    int* periods = new int[2]{ 0, 0 };


    MPI_Comm comm, comm_sub;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &comm);



    int remain_dims[] = { 1, 0 };
    MPI_Cart_sub(comm, remain_dims, &comm_sub);

    MPI_Bcast(&N, 1, MPI_DOUBLE, 0, comm_sub);
    MPI_Bcast(&N, 1, MPI_DOUBLE, 1, comm_sub);
   
    pt << N;
}
