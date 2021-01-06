#include "pt4.h"
#include "mpi.h"
#include <cmath>

int k;              // количество процессов
int r;              // ранг текущего процесса

int m, p, q;        // размеры исходных матриц
int m0, p0, q0;     // размеры блоков матриц
int k0;             // порядок декартовой решетки, равный sqrt(k)

int *a_, *b_, *c_;  // массивы для хранения исходных матриц в главном процессе
int *a, *b, *c;     // массивы для хранения блоков матриц в каждом процессе

MPI_Datatype MPI_BLOCK_A; // тип данных для блока матрицы A
MPI_Datatype MPI_BLOCK_B; // тип данных для блока матрицы B
MPI_Datatype MPI_BLOCK_C; // тип данных для блока матрицы C

MPI_Comm MPI_COMM_GRID = MPI_COMM_NULL;
                // коммуникатор, связанный с двумерной декартовой решеткой


void input() {
	if (r == 0) {
		pt >> m >> q;
		pt >> m0 >> q0;
		c = new int[m0 * q0];
		c_ = new int[m * q];
		for (int i = 0; i < m * q; i++)	c_[i] = 0;
		for (int i = 0; i < m0 * q0; i++) {
			pt >> c[i];
		}
	}
	else {
		pt >> m0 >> q0;
		b = new int[m0 * q0];
		for (int i = 0; i < m0 * q0; i++) {
			pt >> b[i];
		}
	}
}


void Matr3CreateTypeBlock(int m0, int p0, int p, MPI_Datatype& t) {
	MPI_Type_vector(m0, p0, p, MPI_INT, &t);
	MPI_Type_commit(&t);
}

void Matr3GatherData() {

	if (r == 0) {
		MPI_Status status;
		Matr3CreateTypeBlock(m0, q0, q0, MPI_BLOCK_C);

		int ind = 0;
		for (int i = 0; i < m0; i++)
			for (int j = 0; j < q0; j++)
				c_[i * q + j] = c[ind++];

		for (int proc = 1; proc < k; proc++) {
			MPI_Recv(c, 1, MPI_BLOCK_C, proc, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			ind = 0;
			int shift = ((proc / k0) * k0 * q0 / q) * (m0 * q0 * q / q0);
			for (int i = 0; i < m0; i++)
				for (int j = 0; j < q0; j++) {
					if (c[ind])
						c_[shift + i * q + (proc % ((q0 * k0) / q0)) * q0 + j] = c[ind++];
					else
						ind++;
				}
		}

		for (int i = 0; i < m * q; i++) {
			pt << c_[i];
		}
	}
	else {
		Matr3CreateTypeBlock(m0, q0, q0, MPI_BLOCK_C);
		MPI_Send(b, 1, MPI_BLOCK_C, 0, 0, MPI_COMM_WORLD);
	}

}



void Solve()
{
    Task("MPI9Matr27");
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

    input();
	Matr3GatherData();

}
