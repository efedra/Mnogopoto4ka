#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI5Comm1");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int g = size / 2;
	if (size % 2 == 1)
		g++;
	std::vector<int> n(g);
	std::vector<int> res(1);
	if (rank == 0)
	{
		for (int i = 0; i < g; i++)
			pt >> n[i];
	}
	std::vector<int> ranks(g);
	for (int i = 0; i < g; i++)
	{
		ranks[i] = 2 * i;
	}
	MPI_Group og, ng;
	MPI_Comm_group(MPI_COMM_WORLD, &og);
	MPI_Group_incl(og, g, &ranks[0], &ng);
	MPI_Comm nc;
	MPI_Comm_create(MPI_COMM_WORLD, ng, &nc);

	auto scounts = new int[g];
	auto d = new int[g];
	for (int i = 0; i < g; i++)
	{
		scounts[i] = 1;
		d[i] = i;
	}
	MPI_Scatterv(&n[0], scounts, d, MPI_INT, &res[0], 1, MPI_INT, 0, nc);
	if (rank % 2 == 0) {
		for (auto i : res) {
			pt << i;
		}
	}

}
