#include "pt4.h"
#include "mpi.h"

#include <cmath>

int k;              // количество процессов
int r;              // ранг текущего процесса

int m, p, q;        // размеры исходных матриц
int m0, p0, q0;     // размеры блоков матриц
int k0;             // порядок декартовой решетки, равный sqrt(k)

int* a_, * b_, * c_;  // массивы для хранения исходных матриц в главном процессе
int* a, * b, * c;     // массивы для хранения блоков матриц в каждом процессе

MPI_Datatype MPI_BLOCK_A; // тип данных для блока матрицы A
MPI_Datatype MPI_BLOCK_B; // тип данных для блока матрицы B
MPI_Datatype MPI_BLOCK_C; // тип данных для блока матрицы C

MPI_Comm MPI_COMM_GRID = MPI_COMM_NULL;
// коммуникатор, связанный с двумерной декартовой решеткой

void Matr3CreateCommGrid(MPI_Comm* comm)
{
	int dims[2], periods[2];
	dims[0] = dims[1] = k0;
	periods[0] = periods[1] = 1;
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, comm);
}

void Matr3Calc()
{

	for (int i = 0; i < m0; ++i)
		for (int j = 0; j < q0; ++j)
			for (int n = 0; n < p0; ++n)
				c[i * q0 + j] += (a[i * p0 + n] * b[n * q0 + j]);

	int coords[2];
	MPI_Cart_coords(MPI_COMM_GRID, r, 2, coords);

	int rankSource, rankDest;
	MPI_Cart_shift(MPI_COMM_GRID, 0, -1, &rankSource, &rankDest);
	MPI_Cart_shift(MPI_COMM_GRID, 1, -1, &rankSource, &rankDest);

	MPI_Sendrecv_replace(a, m0 * p0, MPI_INT, rankDest, 0, rankSource, 0, MPI_COMM_GRID, MPI_STATUSES_IGNORE);
	MPI_Sendrecv_replace(b, p0 * q0, MPI_INT, rankDest, 0, rankSource, 0, MPI_COMM_GRID, MPI_STATUSES_IGNORE);
}


void Solve()
{
	Task("MPI9Matr25");
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

	pt >> m0 >> p0 >> q0;

	a = new int[m0 * p0];
	for (int i = 0; i < m0; ++i)
		for (int j = 0; j < p0; ++j)
			pt >> a[i * p0 + j];

	b = new int[p0 * q0];
	for (int i = 0; i < p0; ++i)
		for (int j = 0; j < q0; ++j)
			pt >> b[i * q0 + j];

	c = new int[m0 * q0];
	for (int i = 0; i < m0; ++i)
		for (int j = 0; j < q0; ++j)
			pt >> c[i * q0 + j];

	Matr3CreateCommGrid(&MPI_COMM_GRID);
	Matr3Calc();

	for (int i = 0; i < m0; ++i)
		for (int j = 0; j < p0; ++j)
			pt << a[i * p0 + j];

	for (int i = 0; i < p0; ++i)
		for (int j = 0; j < q0; ++j)
			pt << b[i * q0 + j];

	for (int i = 0; i < m0; ++i)
		for (int j = 0; j < q0; ++j)
			pt << c[i * q0 + j];

}