#include "mpi.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
int main(int argc, char* argv[]) {
	int rank, K;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &K);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	srand(time(0) + rank);

	int* send, recv;
	send = new int(K);

	if (rank == 0) {
		for (int i = 0; i < K; i++) {
			send[i] = rand() % 50;
		}
		printf("0 procesinde: ");
		for (int i = 0; i < K; i++) {
			printf("%d ", send[i]);
		}
		printf("\n");
	}

	MPI_Scatter(send, 1, MPI_INT, &recv, 1, MPI_INT, 0, MPI_COMM_WORLD);
	printf("%d procesinde(scatter): %d\n", rank, recv);
	MPI_Finalize();
}
