#include "pt4.h"
#include "mpi.h"
#include <cmath>



class ShowMatrix {
	int* matrix;
	int nRows;
	int nCols;

public:
	ShowMatrix(int* matrix, int nRows, int nCols)
		: matrix(matrix), nRows(nRows), nCols(nCols) { }

	friend PTIO& operator << (PTIO& p, const ShowMatrix& a) {
		for (int i = 0; i < a.nRows; ++i) {
			for (int j = 0; j < a.nCols; ++j) {
				p << a.matrix[i * a.nCols + j];
			}
		}
		return p;
	}
};

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
void SendBlock(int* matrix, int* block, int blockNRows, int blockNCols, int matrixNCols, MPI_Datatype type) {
	if (!r) {
		for (int i = 1; i < k; ++i) {
			int row = i / k0;
			int col = i % k0;
			MPI_Ssend(&matrix[row * blockNRows * matrixNCols + col * blockNCols], 1, type, i, 0, MPI_COMM_WORLD);
		}
		for (int i = 0; i < blockNRows; ++i) {
			memcpy(&block[i * blockNCols], &matrix[i * matrixNCols], blockNCols * sizeof(*block));
		}
	}
	else {
		MPI_Status status;
		MPI_Recv(block, blockNRows * blockNCols, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	}
}

void Matr3CreateTypeBlock(int m0, int p0, int p, MPI_Datatype* type) {
	MPI_Type_vector(m0, p0, p, MPI_INT, type);
	MPI_Type_commit(type);
}


void Matr3ScatterData() {
	int mpq[3] = { m0, p0, q0 };
	MPI_Bcast(mpq, 3, MPI_INT, 0, MPI_COMM_WORLD);
	m0 = mpq[0];
	p0 = mpq[1];
	q0 = mpq[2];
	pt << m0 << p0 << q0;
	a_ = new int[m0 * p0];
	b_ = new int[p0 * q0];
	c_ = new int[m0 * q0];
	memset(c_, 0, m0 * q0 * sizeof(*c_));
	Matr3CreateTypeBlock(m0, p0, p0 * k0, &MPI_BLOCK_A);
	
	Matr3CreateTypeBlock(p0, q0, q0 * k0, &MPI_BLOCK_B);
	
	SendBlock(a, a_, m0, p0, p0 * k0, MPI_BLOCK_A);
	SendBlock(b, b_, p0, q0, q0 * k0, MPI_BLOCK_B);
	pt << ShowMatrix(a_, m0, p0);
	pt << ShowMatrix(b_, p0, q0);
	pt << ShowMatrix(c_, m0, q0);
}


void Solve()
{
    Task("MPI9Matr23");
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

	if (!rank) {
		pt >> m >> p >> q;
		m0 = (int)ceil((double)m / k0);
		p0 = (int)ceil((double)p / k0);
		q0 = (int)ceil((double)q / k0);
		a = new int[(m0 * k0) * (p0 * k0)];
		b = new int[(p0 * k0) * (q0 * k0)];
		for (int i = 0; i < m0 * k0; ++i) {
			for (int j = 0; j < p0 * k0; ++j) {
				if (j < p && i < m) {
					pt >> a[i * p0 * k0 + j];
				}
				else {
					a[i * p0 * k0 + j] = 0;
				}
			}
		}
		for (int i = 0; i < p0 * k0; ++i) {
			for (int j = 0; j < q0 * k0; ++j) {
				if (j < q && i < p) {
					pt >> b[i * q0 * k0 + j];
				}
				else {
					b[i * q0 * k0 + j] = 0;
				}
			}
		}
	}
	Matr3ScatterData();

}
