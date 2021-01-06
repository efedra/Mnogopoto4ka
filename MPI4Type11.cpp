#include "pt4.h"
#include "mpi.h"


typedef struct {
    int a;
    int b;
    double c;
} INDATA_TYPE;


void Solve()
{
    Task("MPI4Type11");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int size_int;
    MPI_Type_size(MPI_INT, &size_int);

    MPI_Datatype type;
    MPI_Type_create_struct(2, new int[2]{ 2, 1 }, new int[2]{ 0, size_int * 2 }, new MPI_Datatype[2]{ MPI_INT, MPI_DOUBLE }, &type);
    MPI_Type_commit(&type);

    int K = size - 1;

    INDATA_TYPE* data = new INDATA_TYPE[K];
    char* packdata = new char[sizeof(INDATA_TYPE) * K];
    int packdata_offset = 0;

    if (rank == 0) {
        for (int i = 0; i < K; ++i) {
            pt >> data[i].a;
            pt >> data[i].b;
            pt >> data[i].c;
        }
        MPI_Pack(data, K, type, packdata, sizeof(INDATA_TYPE) * K, &packdata_offset, MPI_COMM_WORLD);

        delete[] data;

    }
    MPI_Bcast(packdata, sizeof(INDATA_TYPE) * K, MPI_PACKED, 0, MPI_COMM_WORLD);

    if (rank != 0) {
        MPI_Unpack(packdata, sizeof(INDATA_TYPE) * K, &packdata_offset, data, K, type, MPI_COMM_WORLD);

        for (int i = 0; i < K; ++i) {
            pt << data[i].a;
            pt << data[i].b;
            pt << data[i].c;
        }
    }


}
