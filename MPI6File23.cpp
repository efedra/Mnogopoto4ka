#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI6File23");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    MPI_File file;
    char name[12];

    if (rank == 0)
    {
        pt >> name;
    }


    MPI_Bcast(name, 12, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_File_open(MPI_COMM_WORLD, name, MPI_MODE_RDONLY, MPI_INFO_NULL, &file);

    MPI_Datatype t;


    MPI_Type_create_resized(MPI_DOUBLE, 0, sizeof(double), &t);

    double* result1 = new double[3];
    double* result2 = new double[3];


    char* c = new char[7]{ 'n', 'a', 't', 'i', 'v', 'e', '\0' };
    MPI_File_set_view(file, 3 * (rank) * sizeof(double), MPI_DOUBLE, t, c, MPI_INFO_NULL);
    MPI_File_read_all(file, result1, 3, MPI_DOUBLE, MPI_STATUS_IGNORE);
    MPI_File_set_view(file, 3 * (rank + size) * sizeof(double), MPI_DOUBLE, t, c, MPI_INFO_NULL);


    MPI_File_read_all(file, result2, 3, MPI_DOUBLE, MPI_STATUS_IGNORE);



    pt <<   result1[0];
    pt <<   result1[1];
    pt <<   result1[2];
    pt <<   result2[0];
    pt <<    result2[1];
    pt <<   result2[2];
}
