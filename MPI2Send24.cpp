#include "pt4.h"
#include "mpi.h"

template<typename T>
void sendRecv(int rank, int size, MPI_Datatype d, int rankDestination) {
    int N = size / 2;
    T* numbers = new T[N];

    for (int i = 0; i < N; ++i) {
        T number;
        pt >> number;
        numbers[i] = number;
    }

    MPI_Status status;
    MPI_Sendrecv_replace(numbers, N, d, rankDestination, 0, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

    for (int i = 0; i < N; ++i)
        pt << numbers[i];
}

void Solve()
{
    Task("MPI2Send24");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    rank % 2 ? sendRecv<int>(rank, size, MPI_INT, rank == 1 ? size - 1 : rank - 2) : sendRecv<double>(rank, size, MPI_DOUBLE, rank == size - 2 ? 0 : rank + 2);

}


