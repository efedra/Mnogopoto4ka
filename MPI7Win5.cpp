#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI7Win5");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);



    MPI_Win form;
    int* a = new int[size - 1];
    int* a0 = new int[size - 1];
    MPI_Win_create(a, (size - 1) * sizeof(int), sizeof(int), MPI_INFO_NULL,
        MPI_COMM_WORLD, &form);

    if (rank == 0)
        for (int i = 0; i < size - 1; i++)
            pt >> a[ i ];


    MPI_Win_fence(0, form);
    if (rank > 0)
    {
        int k, c;
        pt >> k >> c;
        for (int i = 0; i < size - 1; ++i)
            a0[i] = 1;
        a0[k] = c;

            MPI_Accumulate(a0,  size - 1,  MPI_INT , 0, 0, size - 1,

            MPI_INT, MPI_PROD, form);


    }


    MPI_Win_fence(0 , form );

    if (rank == 0)
    {
        for (int i = 0; i < size - 1; ++i)
        {
            pt << a[i];
            Show(a[i]);
        }
    }

    MPI_Win_free(&form);
}
