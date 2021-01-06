#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI3Coll25");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double* numbers = new double[2 * size];
    double* result = new double[2];
    int* rcounts = new int[size];

    for (int i = 0; i < size; ++i)
        rcounts[i] = 2;

    for (int i = 0; i < size * 2; ++i)
        pt >> numbers[i];

    MPI_Reduce_scatter(numbers, result, rcounts, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
    for (int i = 0; i < 2; ++i)
		pt << result[i];



}
