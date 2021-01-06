#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI4Type21");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int K = size - 1;
	double* matrix = new double[K * K];
	MPI_Datatype newtype;
	MPI_Type_vector(K, 1, K, MPI_DOUBLE, &newtype);
	MPI_Type_commit(&newtype);

	if (rank == 0) {
		for (int i = 0; i < K * K; ++i)
			pt >> matrix[i];
	}
	int* sendcounts = new int[size];
	int* sdispls = new int[size];
	MPI_Datatype* sendtypes = new MPI_Datatype[size];
	int* recvcounts = new int[size];
	int* rdispls = new int[size];
	MPI_Datatype* recvtypes = new MPI_Datatype[size];

	if (rank == 0) {
		sendcounts[0] = 0;
		sdispls[0] = 0;
		std::fill(sendtypes, sendtypes + size, newtype);
		for (int i = 1; i < size; i++) {
			sdispls[i] = (i - 1) * sizeof(double);
			sendcounts[i] = 1;
		}
		std::fill(rdispls, rdispls + size, 0);
		std::fill(recvcounts, recvcounts + size, 0);
		std::fill(recvtypes, recvtypes + size, newtype);
	}
	else {
		sendcounts[0] = 0;
		sdispls[0] = 0;
		std::fill(sendtypes, sendtypes + size, newtype);
		std::fill(sendcounts, sendcounts + size, 0);
		std::fill(sdispls, sdispls + size, 0);
		std::fill(rdispls, rdispls + size, 0);
		std::fill(recvcounts, recvcounts + size, 0);
		recvcounts[0] = K;
		std::fill(recvtypes, recvtypes + size, MPI_DOUBLE);
	}
	double* recv = new double[K];
	MPI_Alltoallw(matrix, sendcounts, sdispls, sendtypes, recv, recvcounts, rdispls, recvtypes, MPI_COMM_WORLD);
	if (rank != 0)
		for (int i = 0; i < K; ++i)
			pt << recv[i];

}
