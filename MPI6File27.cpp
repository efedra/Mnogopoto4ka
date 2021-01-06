#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI6File27");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    char name[12];
    if (rank == 0)
    {
        pt >> name;

    }

    MPI_Bcast(name, 12, MPI_CHAR, 0, MPI_COMM_WORLD);


    int  dbl_sz;
    MPI_Type_size( MPI_DOUBLE,  &dbl_sz);



    MPI_File file;
    MPI_File_open(MPI_COMM_WORLD , name , MPI_MODE_RDONLY , MPI_INFO_NULL , &file);


    MPI_Datatype t;
    MPI_Type_create_resized(MPI_DOUBLE, 0, size * dbl_sz, &t);
    MPI_Type_contiguous(size / 2, t, &t);

    char* c = new char[7]{ 'n', 'a', 't', 'i', 'v', 'e', '\0' };

    MPI_File_set_view(file,     rank * dbl_sz, MPI_DOUBLE, t , c , MPI_INFO_NULL);



    double* arr = new double[size / 2];
    MPI_File_read_all(file, arr, size / 2, MPI_DOUBLE, MPI_STATUS_IGNORE);

    for (int i = 0; i < size / 2; ++i)
    {
        pt << arr[i];
    }

}
