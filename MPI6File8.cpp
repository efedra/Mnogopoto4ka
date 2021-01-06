#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI6File8");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);



    char N  [20];

    if (rank == 0)
    {
        pt >> N;

    }

    double n = 0;
    if (rank != 0)
    {
        pt >> n;
    }


    MPI_Bcast(  N, 20,    MPI_CHAR,   0,  MPI_COMM_WORLD);


    MPI_File file;
    MPI_File_open(MPI_COMM_WORLD, N, MPI_MODE_WRONLY | MPI_MODE_CREATE, MPI_INFO_NULL, &file);

    int ranks = rank * (rank + 1) / 2;
    int dbl_sz;
    MPI_Type_size(MPI_DOUBLE, &dbl_sz);


    MPI_Offset offset = (((size - 1) * size / 2) - ranks) * dbl_sz;

    double* arr = new double[rank];
    for (int i = 0; i < rank; i++)
        arr[i] = n;

    MPI_File_seek(file, offset, MPI_SEEK_SET);

    MPI_File_write(file, arr, rank, MPI_DOUBLE, MPI_STATUS_IGNORE);

}
