#include "mpi.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) 
{
	double x[12] = {1,2,3,4,5,6,7,8,9,10,11,12 }, y[12] = {1,2,3,4,5,6,7,8,9,10,11,12};
	double res = 0, p_res = 0.0;
	int n, rank, count, i, N = 12;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &count);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	for (i = rank * N / count; i < (rank + 1) * N / count; i++) 
	{
		p_res = p_res + x[i] * y[i];
		printf("answer: %10.2f vec1: %10.2f, vec2: %10.2f\n", p_res, x[i], y[i]);
	}

	int nextRank = rank + 1;
	if (rank == count - 1) nextRank = 0;

	int prevRank = rank - 1;
	if (rank == 0) prevRank = count - 1;

	if (rank == 0) {
		res = p_res;
		MPI_Send(&res, 1, MPI_DOUBLE, nextRank, 0, MPI_COMM_WORLD);
		MPI_Recv(&res, 1, MPI_DOUBLE, prevRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	else {
		MPI_Recv(&res, 1, MPI_DOUBLE, prevRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		res = res + p_res;
		MPI_Send(&res, 1, MPI_DOUBLE, nextRank, 0, MPI_COMM_WORLD);
	}

	printf("\nInner product = %10.2f \n\n", res);

	MPI_Finalize();
}
