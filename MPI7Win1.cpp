#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI7Win1");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    
    MPI_Win form;

    int* base = new int[size - 1];
    MPI_Win_create(base, rank == 0 ? (size - 1) * sizeof(int) : 0, sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &form);


    MPI_Win_fence(0, form);
    if (rank != 0)
    {
        int  k;

        pt >> k ;

        MPI_Put(&k  , 1, MPI_INT, 0, rank - 1, 1 , MPI_INT, form );
    }

    MPI_Win_fence(0, form);

    if (rank == 0)
        for (int i  = 0; i < size - 1; ++i)
        {
            pt <<    base[ i ];
        }
    

}
