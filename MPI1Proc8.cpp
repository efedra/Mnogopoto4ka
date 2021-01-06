#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI1Proc8");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
     
    int n;
    pt >> n; 
   
    std::vector<double> res;

    for (int i = 1; i <= n; i++)
    {
        double sub=0;
        pt >> sub;
        res.push_back(sub);
    }

   /* double sum = 0;
    for (int i = 0; i <= res.size()-1; i++)
    {
        sum += res[i];
    }

    if (rank % 2 == 0)
    {
        pt << sum;
    }
    else
    {
        pt << sum / n;*/
   // }

}
