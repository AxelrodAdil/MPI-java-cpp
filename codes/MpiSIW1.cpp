#include "mpi.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
//////////////////////////////////////////////////////////////////////////
int* createSubMatrixColLen(int max, int numberOfBuckets) {
    int e = max / numberOfBuckets;
    vector<int> b(numberOfBuckets, e);
    fill(b.begin(), b.begin() + (max % numberOfBuckets), e + 1);
    int* arr = new int[numberOfBuckets];
    std::copy(b.begin(), b.end(), arr);
    return arr;
}
//////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
    //main init
    int size, rank;
    int colLen = 12;
    int rowLen = 6;
    int rootRank = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //MPI_Scatter init
    int* displs = new int[size];
    int* sendcounts = new int[size];
    int* recvcounts = new int[size];

    int* subMatrixColLen = createSubMatrixColLen(colLen, size);
    displs[0] = 0;
    for (int i = 1; i <= size - 1; i++) {
        sendcounts[i - 1] = recvcounts[i - 1] = subMatrixColLen[i - 1];
        displs[i] = displs[i - 1] + sendcounts[i - 1];
    }
    sendcounts[size - 1] = recvcounts[size - 1] = subMatrixColLen[size - 1];

    if (rank == rootRank) {
        for (int i = 0; i < size; i++) {
            printf("\nsendcounts: [%d]: %d \n", i, sendcounts[i]);
            printf("recvcounts: [%d]: %d \n", i, recvcounts[i]);
            printf("displacements: [%d]: %d \n", i, displs[i]);
        }
    }

    //matrix and vector init
    int** matrix = new int* [rowLen];
    int* vector = new int[colLen];
    int* finalVector = new int[colLen];

    srand(rank);
    if (rank == rootRank) {
        printf("\n\nMatrix: \n");
        for (int i = 0; i < rowLen; i++) {
            matrix[i] = new int[colLen];

            for (int j = 0; j < colLen; j++) {
                matrix[i][j] = rand() % 100;
            }
        }

        for (int i = 0; i < colLen; i++) {
            vector[i] = rand() % 10;
        }

        for (int i = 0; i < rowLen; i++) {
            for (int j = 0; j < colLen; j++) {
                printf("[ %d ] ", matrix[i][j]);
            }
            printf("\n");
        }
    }

    double time = MPI_Wtime();
    //root rank will send the vector to others
    MPI_Bcast(vector, colLen, MPI_INT, rootRank, MPI_COMM_WORLD);

    printf("\n\nProcess %d vector: \n", rank);
    for (int i = 0; i < colLen; i++) {
        printf("[ %d ] ", vector[i]);
    }

    //receive buffer init
    int recvBufLen = recvcounts[rank];
    int** recvBuf = new int* [rowLen];
    for (int i = 0; i < rowLen; i++) {
        recvBuf[i] = new int[recvBufLen];
    }

    //root rank will send the matrix to others
    for (int row = 0; row < rowLen; row++) {
        MPI_Scatterv(matrix[row], sendcounts, displs, MPI_INT, recvBuf[row], recvcounts[rank], MPI_INT, rootRank, MPI_COMM_WORLD);
    }

    //each pross submatrix data
    printf("\nProcess %d sub matrix: \n", rank);
    printf("Values: [%d] \n", recvcounts[rank]);
    for (int i = 0; i < rowLen; i++) {
        for (int j = 0; j < recvBufLen; j++) {
            printf("[ %d ] ", recvBuf[i][j]);
        }
        printf("\n");
    }


    //each pross subvector data
    printf("\nProcess %d sub vector: \n", rank);
    int* subVector = new int[rowLen];
    for (int i = 0; i < rowLen; i++) {
        int sum = 0;
        for (int j = 0; j < recvBufLen; j++) {
            sum += (recvBuf[i][j] * vector[displs[rank] + j]);
        }
        subVector[i] = sum;
        printf("[%d]\n", sum);
    }
    printf("\n\n");

    //each pross will sand their sub vector to root pross
    MPI_Reduce(subVector, finalVector, rowLen, MPI_INT, MPI_SUM, rootRank, MPI_COMM_WORLD);

    time = MPI_Wtime() - time;

    //final vector show
    if (rank == rootRank) {
        printf("Final Vector: \n");
        for (int i = 0; i < rowLen; i++) {
            printf("[%d]\n", finalVector[i]);
        }

        cout << "\n\n Time: " << time << "\n";
    }


    MPI_Finalize();
    return EXIT_SUCCESS;
}
