#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI6File12");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char N[20];
    if (rank == 0)
    {
        pt >> N;
    }

    MPI_Bcast(N, 20, MPI_CHAR, 0, MPI_COMM_WORLD);



    int n;
    pt >>    n;

    MPI_Comm c;
    MPI_Comm_split(MPI_COMM_WORLD, n == 0 ? MPI_UNDEFINED : 1, rank, &c);
    if (c == MPI_COMM_NULL)
    {
        return;
    }
    MPI_Comm_size(c, &size);
    MPI_Comm_rank(c, &rank);

    Show( rank );


    MPI_File file;
    MPI_File_open(c, N, MPI_MODE_WRONLY, MPI_INFO_NULL, &file);

    int dbl_sz;
    MPI_Type_size(MPI_DOUBLE, &dbl_sz);

    double r = rank;
    MPI_Offset off_set = (n - 1) * dbl_sz;

    MPI_File_write_at_all(file, off_set, &r,1, MPI_DOUBLE, MPI_STATUS_IGNORE);
   



}
