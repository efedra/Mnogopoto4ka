#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI5Comm12");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int N;
    double input;
    double output;
    pt >> N;
    pt >> input;


    MPI_Comm comm;



    MPI_Comm_split(MPI_COMM_WORLD, N, rank, &comm);



    MPI_Allreduce(&input, &output, 1, MPI_DOUBLE, N == 1 ? MPI_MIN : MPI_MAX, comm);

    pt << output;

}
