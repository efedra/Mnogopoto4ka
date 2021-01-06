#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI2Send16");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  
   
    int N;
    pt >> N;
    MPI_Status s;
    if (N != 0)
    {
        
        for (int i = 0; i <size; i++)
        {
            if (i!=rank) {
                double in;
                pt >> in;
                MPI_Send(&in, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            }
        }
      
    }
    else
    {
        double out;
        MPI_Recv(&out, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &s);
        pt << out;
    }
 
   

}
