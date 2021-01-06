#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI4Type18");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    if (rank == 0) {
        int N;
        pt >> N;

        int matrix_size = (3 * N) * ((size - 1) / 3 * N);
        int* matrix = new int[matrix_size];
        for (int i = 0; i < matrix_size; ++i)
            pt >> matrix[i];

        int* blocklens = new int[N];
        int* displs = new int[N];
        for (int i = 0; i < N; ++i) {
            blocklens[i] = i + 1;
            displs[i] = i * N * 3;
        }
        MPI_Datatype type_block;
        MPI_Type_indexed(N, blocklens, displs, MPI_INT, &type_block);
        MPI_Type_commit(&type_block);

        int q = 0;
        for (int i = 1; i < size; ++i) {
            int offset = (i - 1) * N;


            if (i % 3 == 1 && i != 1) {
                q += 1;
            }
            offset += N * 3 * (N - 1) * q;
            MPI_Send(matrix + offset, 1, type_block, i, 0, MPI_COMM_WORLD);
        }


    }
    else
    {
        int block_size;

        MPI_Status status;
        MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &block_size);
        int* input_block = new int[block_size];

        MPI_Recv(input_block, block_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        for (int i = 0; i < block_size; ++i)
            pt << input_block[i];

    }

}
