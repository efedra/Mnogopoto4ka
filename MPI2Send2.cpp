#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI2Send2");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


   

    if (rank==0)
    {
        for (int i = size - 1; i > 0; --i) {
            double N;
            MPI_Status status;
            MPI_Recv(&N, 1, MPI_DOUBLE, i, i, MPI_COMM_WORLD, &status);
            pt << N;
        }
    }
    if(rank!=0)
    {
        double N;
        pt >> N;
        int buffsize = sizeof(double)    +   MPI_BSEND_OVERHEAD;
        double* buff = (double*)malloc(buffsize);
        MPI_Buffer_attach(buff, buffsize);
        MPI_Bsend(&N,1,MPI_DOUBLE,0,rank,MPI_COMM_WORLD);
        MPI_Buffer_detach(buff, &buffsize);
    }
    
}
