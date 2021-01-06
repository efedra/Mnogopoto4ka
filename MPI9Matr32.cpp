#include "pt4.h"
#include "mpi.h"
#include <cmath>

int k;              // ���������� ���������
int r;              // ���� �������� ��������

int m, p, q;        // ������� �������� ������
int m0, p0, q0;     // ������� ������ ������
int k0;             // ������� ���������� �������, ������ sqrt(k)

int *a_, *b_, *c_;  // ������� ��� �������� �������� ������ � ������� ��������
int *a, *b, *c, *t; // ������� ��� �������� ������ ������ � ������ ��������

MPI_Datatype MPI_BLOCK_A; // ��� ������ ��� ����� ������� A
MPI_Datatype MPI_BLOCK_B; // ��� ������ ��� ����� ������� B
MPI_Datatype MPI_BLOCK_C; // ��� ������ ��� ����� ������� C

MPI_Comm MPI_COMM_GRID = MPI_COMM_NULL;
                // ������������, ��������� � ��������� ���������� ��������
MPI_Comm MPI_COMM_ROW = MPI_COMM_NULL;
                // ������������, ��������� �� �������� ��������� �������
MPI_Comm MPI_COMM_COL = MPI_COMM_NULL;
                // ������������, ��������� �� ��������� ��������� �������


void Matr4CreateTypeBlock(int m0, int p0, int p, MPI_Datatype* t) {
    MPI_Type_vector(m0, p0, p, MPI_INT, t);
    MPI_Type_commit(t);
}


void Solve()
{
    Task("MPI9Matr32");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    k = size;
    r = rank;
    k0 = (int)floor(sqrt((double)k) + 0.1);

    pt >> m >> p;


    if (!rank) {
        a = new int[m * p];
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < p; ++j) {
                pt >> a[i * p +j];
            }
        }
    }


    m0 = m / k0;
    p0 = p / k0;
    Matr4CreateTypeBlock(m0, p0, p, &MPI_BLOCK_A);

    a_ = new int[m0 * p0];
    int* sendcounts = new int[k];
    int* sdispls = new int[k];
    MPI_Datatype* sendtypes = new MPI_Datatype[k];
    int* recvcounts = new int[k];
    int* rdispls = new int[k];
    MPI_Datatype* recvtypes = new MPI_Datatype[k];



    for (int i = 0; i < k; ++i) {
        sendcounts[i] = !rank ? 1 : 0;
        int row = i / k0;
        int col = i % k0;
        sdispls[i] = (row * m0 * p + col * p0) * sizeof(int);
        sendtypes[i] = MPI_BLOCK_A;
        recvcounts[i] = !i ? m0 * p0 : 0;
        rdispls[i] = 0;
        recvtypes[i] = MPI_INT;
    }

    MPI_Alltoallw(a, sendcounts, sdispls, sendtypes, a_, recvcounts, rdispls, recvtypes, MPI_COMM_WORLD);


    for (int i = 0; i < m0; ++i) {
        for (int j = 0; j < p0; ++j) {
            pt << a_[i * p0 + j];
        }
    }

}
