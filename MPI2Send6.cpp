#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI2Send6");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


   
    MPI_Status s;
    if (rank == 0)
    {
        int buffsize = sizeof(double) * size + MPI_BSEND_OVERHEAD;
        int* buff = (int*)malloc(buffsize);
        MPI_Buffer_attach(buff, buffsize);
        for (int i = (size - 1); i != 0; i--)
        {
            double in;
            pt >> in;
            MPI_Bsend(&in, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
        MPI_Buffer_detach(buff, &buffsize);
    }
    else
    {
        double out;
        MPI_Recv(&out, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &s);
        pt << out;
    }
}
