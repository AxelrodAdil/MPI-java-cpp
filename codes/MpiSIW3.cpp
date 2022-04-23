#include "mpi.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Status status;
	srand(time(0));

	const int ndims = 2;
	int dim_size[ndims], periods[ndims];
	dim_size[0] = size; dim_size[1] = 1;
	periods[0] = 1; periods[1] = 0;

	MPI_Comm COMM_CART;
	MPI_Cart_create(MPI_COMM_WORLD, ndims, dim_size, periods, 0, &COMM_CART);

	int next_rank, prev_rank;
	MPI_Cart_shift(COMM_CART, 0, 1, &next_rank, &prev_rank);

	int* A = new int[size * size];
	int* B = new int[size * size];
	int* Product = new int[size * size];

	int* Arows = new int[size];
	int* Brows = new int[size];

	if (rank == 0) {
		printf("A\n");
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				A[i * size + j] = rand() % 10;
				printf("%3d\t", A[i * size + j]);
			}
			printf("\n");
		}

		printf("B\n");
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				B[i * size + j] = rand() % 10;
				printf("%3d\t", B[i * size + j]);
			}
			printf("\n");
		}
	}

	MPI_Scatter(A, size, MPI_INT, Arows, size, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(B, size, MPI_INT, Brows, size, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 1; i <= size; i++) {
		MPI_Sendrecv_replace(Brows, size, MPI_INT, next_rank, 0, prev_rank, 0, MPI_COMM_WORLD, &status);
		int Aelement = (rank + i) % size;
		for (int j = 0; j < size; j++) {
			Product[Aelement * size + j] = Arows[Aelement] * Brows[j];
		}
	}

	printf("%d row: ", rank);
	for (int i = 0; i < size; i++) {
		int sum = 0;
		for (int j = 0; j < size; j++) {
			sum += Product[size * j + i];
		}
		printf("%d ", sum);
	}
	printf("\n");
	MPI_Finalize();

	return 0;
}
