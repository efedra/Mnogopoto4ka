#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI6File20");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    char name [ 12 ];
    if (rank == 0)
    {
        pt >> name;
    }

    MPI_Bcast(name , 12, MPI_CHAR, 0, MPI_COMM_WORLD);



    MPI_File file ;
    MPI_File_open(MPI_COMM_WORLD, name,  MPI_MODE_CREATE |  MPI_MODE_WRONLY, MPI_INFO_NULL , &file);

    double elements[ 50 ];
    for (int i = rank; i >= 0; --i)
        pt >> elements[i];


    
    

    double n;
    if (rank == 0)
    {
        n = ((1.0 + (double)size) / 2.0 * (double)size - 1);
    }
    else
    {
        n = ((1.0 + (double)size) / 2.0 * (double)size - ((double)rank + 1) - ((1 + (double)rank) / 2 * (double)rank));
    }

    char str[6] = { 'n','a','t','i','v','e' };

    MPI_Offset off_set = n;
    Show((int)off_set);

    MPI_File_set_view(file, n * sizeof(double), MPI_DOUBLE, MPI_DOUBLE, const_cast<char*>("native"), MPI_INFO_NULL);
    MPI_File_write_all(file, elements, rank + 1, MPI_DOUBLE, MPI_STATUS_IGNORE);

    MPI_File_close(&file);

}
