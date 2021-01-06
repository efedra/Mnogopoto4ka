#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI6File10");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    MPI_File file;
    char N[12];
    int n;
    int* result = new int[rank];



    if (rank == 0)
        pt >> N;


    int off_set = 0;

    for (size_t i = 0; i < rank; ++i)
    {
        pt >> n;
        result[i] = n;
        off_set += i;
    }

    MPI_Bcast(N, 12, MPI_CHAR, 0, MPI_COMM_WORLD);

    MPI_File_open(MPI_COMM_WORLD, N, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &file);

   

    int int_sz;
    MPI_Type_size(MPI_INTEGER, &int_sz);

    MPI_File_seek(file, off_set * int_sz, MPI_SEEK_SET);


    MPI_File_write_all(file, result, rank, MPI_INT, MPI_STATUS_IGNORE);
}
