#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI3Coll17");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    	auto arr = new double[size +1];
	for (int i = 0; i < size + 1; i++)
	{
		pt >> arr[i];
	}
	auto scounts = new int[size];
	auto rcounts = new int[size];
	auto sdispls = new int[size];
	for (int i = 0; i < size; i++)
	{
		scounts[i] = 2;
		rcounts[i] = size*2;
		sdispls[i] = i;
	}
	auto rdispls = new int[size];
	for (int i = 0; i < size; i++)
	{
		rdispls[i] = i * 2;
	}
	auto rbuf = new double[size * 2];
	MPI_Alltoallv(arr, scounts, sdispls, MPI_DOUBLE, rbuf, rcounts, rdispls, MPI_DOUBLE, MPI_COMM_WORLD);
	for (int i = 0; i < size * 2; i++)
	{
		pt << rbuf[i];
	}


}
