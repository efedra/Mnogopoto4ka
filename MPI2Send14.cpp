#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI2Send14");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int input1, input2, output1, output2;
	MPI_Status status;

	pt >> output1;
	pt >> output2;

	if (rank > 0) {
		MPI_Ssend(&output1, 1, MPI_INT, rank == 0 ? size - 1 : rank - 1, 0, MPI_COMM_WORLD);
		MPI_Recv(&input2, 1, MPI_INT, rank == size - 1 ? 0 : rank + 1, 0, MPI_COMM_WORLD, &status);

		MPI_Ssend(&output2, 1, MPI_INT, rank == size - 1 ? 0 : rank + 1, 0, MPI_COMM_WORLD);
		MPI_Recv(&input1, 1, MPI_INT, rank == 0 ? size - 1 : rank - 1, 0, MPI_COMM_WORLD, &status);

		pt << input1;
		pt << input2;
	}
	else {
		MPI_Recv(&input2, 1, MPI_INT, rank == size - 1 ? 0 : rank + 1, 0, MPI_COMM_WORLD, &status);
		MPI_Ssend(&output1, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD);

		MPI_Recv(&input1, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, &status);
		MPI_Ssend(&output2, 1, MPI_INT, rank == size - 1 ? 0 : rank + 1, 0, MPI_COMM_WORLD);

		pt << input1;
		pt << input2;
	}
    
}
