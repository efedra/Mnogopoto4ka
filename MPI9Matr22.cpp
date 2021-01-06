#include "pt4.h"
#include "mpi.h"
#include <cmath>

int k;              // ���������� ���������
int r;              // ���� �������� ��������

int m, p, q;        // ������� �������� ������
int m0, p0, q0;     // ������� ������ ������
int k0;             // ������� ���������� �������, ������ sqrt(k)

int *a_, *b_, *c_;  // ������� ��� �������� �������� ������ � ������� ��������
int *a, *b, *c;     // ������� ��� �������� ������ ������ � ������ ��������

MPI_Datatype MPI_BLOCK_A; // ��� ������ ��� ����� ������� A
MPI_Datatype MPI_BLOCK_B; // ��� ������ ��� ����� ������� B
MPI_Datatype MPI_BLOCK_C; // ��� ������ ��� ����� ������� C

MPI_Comm MPI_COMM_GRID = MPI_COMM_NULL;
                // ������������, ��������� � ��������� ���������� ��������


void Matr3CreateCommGrid(MPI_Comm* comm) {
    int dims[] = { k0, k0 };
    int periods[] = { TRUE, TRUE };
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, FALSE, comm);
}


void Solve()
{
    Task("MPI9Matr22");
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


    pt >> m0 >> p0;
    a_ = new int[m0 * p0];
    for (int i = 0; i < m0; ++i) {
        for (int j = 0; j < p0; ++j) {
            pt >> a_[i* p0 + j];
        }
    }


    Matr3CreateCommGrid(&MPI_COMM_GRID);
    int newRank;
    int coords[2];
    MPI_Comm_rank(MPI_COMM_GRID, &newRank);
    MPI_Cart_coords(MPI_COMM_GRID, rank, 2, coords);
    pt << coords[0] << coords[1];


    int source, dest;
    MPI_Cart_shift(MPI_COMM_GRID, 1, -coords[0], &source, &dest);
    MPI_Status status;
    MPI_Sendrecv_replace(a_, m0 * p0, MPI_INT, dest, 0, source, 0, MPI_COMM_GRID, &status);


    for (int i = 0; i < m0; ++i) {
        for (int j = 0; j < p0; ++j) {
            pt << a_[i * p0 + j];
        }
    }


}
