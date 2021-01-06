#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI2Send18");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int N;
    pt >> N;
    double inData[4], outData[4] = { 0,0,0,0 };

    for (int i = 0; i < N; ++i) {
        double num;
        pt >> num;
        outData[i] = num;
    }
    int processNumber = rank % 2 == 0 ? rank + 1 : rank - 1;
    
    MPI_Status status;
    MPI_Sendrecv(&outData, 4, MPI_DOUBLE, processNumber, 0, &inData, 4, MPI_DOUBLE, processNumber, 0, MPI_COMM_WORLD, &status);
    for (int i = 0; i < 4; ++i) {
        if (inData[i] == 0)
            break;

        pt << inData[i];
    }
}
