#include "mpi.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	int main = 0;
	int count, rank;
	double number;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &count);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == main) {

		for (int i = 1; i < 4; i++) {
			srand(i);
			number = rand();
			printf("\n proccess %d sent: to %d - %10.2f \n", main, i, number);

			MPI_Send(&number, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
		}
	}
	else {
		MPI_Recv(&number, 1, MPI_DOUBLE, main, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("\n proccess %d received - %10.2f \n\n", rank, number);
	}

	MPI_Finalize();
}
