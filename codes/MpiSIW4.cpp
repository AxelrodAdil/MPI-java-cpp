#include "mpi.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
int main(int argc, char* argv[])
{
	int rank, size;
	int a_cols, a_rows, b_cols, b_rows, a_size, b_size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	srand(time(0));

	//размер матрицы
	a_rows = size;
	a_cols = 2 + rand() % 8;
	b_rows = a_cols;
	b_cols = size;
	a_size = a_rows * a_cols;
	b_size = b_rows * b_cols;

	//определим матрицы
	int* a_matrix = new int[a_rows * a_cols];
	int* b_matrix = new int[b_rows * b_cols];
	int* process_rows = new int[a_cols];
	int* process_cols = new int[b_rows];
	int* const_process_cols = new int[b_rows];


	//создадим новый тип данных для столбцов
	MPI_Datatype COL, MPI_COL;
	MPI_Type_vector(b_rows, 1, b_cols, MPI_INT, &COL);
	MPI_Type_create_resized(COL, 0, sizeof(int), &MPI_COL);
	MPI_Type_commit(&MPI_COL);

	//определим элементы матрицы
	if (rank == 0) {
		printf("Matrix A\n");
		for (int i = 0; i < a_rows; i++) {
			for (int j = 0; j < a_cols; j++) {
				a_matrix[i * a_cols + j] = rand() % 15;
				printf("%3d\t", a_matrix[i * a_cols + j]);
			}
			printf("\n");
		}

		printf("Matrix B\n");
		for (int i = 0; i < b_rows; i++) {
			for (int j = 0; j < b_cols; j++) {
				b_matrix[i * b_cols + j] = rand() % 15;
				printf("%3d\t", b_matrix[i * b_cols + j]);
			}
			printf("\n");
		}
	}
	//распределим по процессам
	MPI_Scatter(a_matrix, a_cols, MPI_INT, process_rows, a_cols, MPI_INT, 0, MPI_COMM_WORLD);
	printf("A: ");
	for (int i = 0; i < a_cols; i++) {
		printf("%3d ", process_rows[i]);
	}
	printf("\n");

	MPI_Scatter(b_matrix, 1, MPI_COL, const_process_cols, b_rows, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < b_rows; i++) {
		process_cols[i] = const_process_cols[i];
	}


	/*printf("Rank %d B: ", rank);
	for (int i = 0; i < b_rows; i++) {
		printf("%3d ", process_cols[i]);
	}
	printf("\n");*/

	//умножаем
	int sum = 0;
	printf("B: ");
	for (int i = 0; i < b_rows; i++) {
		printf("%3d ", process_cols[i]);
	}
	for (int i = 0; i < a_cols; i++) {
		sum += process_rows[i] * process_cols[i];
	}
	printf("[%d] = %d\n", rank, sum);

	int next = (rank + 1) % size;
	int prev = (rank != 0) ? rank - 1 : size - 1;

	while (next != rank) {
		MPI_Sendrecv_replace(process_cols, b_rows, MPI_INT, next, 0, prev, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		printf("B: ");
		for (int i = 0; i < b_rows; i++) {
			printf("%3d ", process_cols[i]);
		}
		sum = 0;
		for (int i = 0; i < a_cols; i++) {
			sum += process_rows[i] * process_cols[i];
		}
		printf("[%d] = %d\n", rank, sum);
		next = (next + 1) % size;
		if (prev != 0) {
			prev--;
		}
		else {
			prev = size - 1;
		}

	}
	printf("\n");
	MPI_Finalize();

	return 0;
}
