#include "pt4.h"
#include "mpi.h"
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;

void Solve()
{
    Task("MPI6File2");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);



    char* name = new char[12];
    

    if (rank == 0)
        pt >> name;
    MPI_Bcast(name, 12, MPI_CHAR, 0, MPI_COMM_WORLD);




    int cnt;
    int n;
    int vl;
    MPI_Status s;

    MPI_File f;
    MPI_File_open(MPI_COMM_WORLD, name, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &f);
    if (rank != 0)
    {
        pt >> cnt;
        for (int i = 0; i < cnt; i++)
        {
            pt >> n;
            pt >> vl;
            int int_sz;
            MPI_Type_size(MPI_INT, &int_sz);
            MPI_Datatype t;
            MPI_Type_create_resized(MPI_INT, 0, (n - 1) * int_sz, &t);
            int* buf = new int[1]{ vl };

           
            MPI_File_write_at(f, (n - 1) * int_sz, buf, 1, t, &s);
        }
    }
    MPI_File_close(&f);
}
