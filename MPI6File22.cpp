#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI6File22");
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



    MPI_File file;

    MPI_Bcast(  name , 12, MPI_CHAR, 0,  MPI_COMM_WORLD);

    MPI_File_open(MPI_COMM_WORLD, name, MPI_MODE_CREATE  | MPI_MODE_WRONLY, MPI_INFO_NULL, &file);

    int int_sz;
    MPI_Type_size(MPI_INTEGER, &int_sz);

    MPI_Datatype t;
    MPI_Type_create_resized(MPI_INT, 0, (size)  * int_sz, &t );

    int* arr = new int[3];
    pt >> arr[0];
    pt >> arr[1];
    pt >> arr[2];


    char* c = new char[7]{ 'n', 'a', 't', 'i', 'v', 'e', '\0' };


    MPI_File_set_view(file  , (size - 1 - rank) * int_sz, MPI_INT, t,  c, MPI_INFO_NULL );
    MPI_File_write_all(file, arr, 3, MPI_INT, MPI_STATUS_IGNORE);
}
