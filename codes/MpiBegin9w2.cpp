#include "mpi.h"
#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    //main init
    int size, rank;
    int matrixLen = 4;
    int matrix[4][4] =
    {
        {1,2,3,4},
        {12,13,14,15},
        {24,24,25,26},
        {34,35,36,37},
    };

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int eachProccItemLen = (matrixLen / size) * matrixLen;
    int colItem = matrixLen / size;

    //init options
    int* sendcounts = new int[size];
    int* recvcounts = new int[size];
    int* displacements = new int[size];

    displacements[0] = 0;
    for (int i = 1; i <= size - 1; i++) {
        sendcounts[i - 1] = colItem;
        recvcounts[i - 1] = colItem;
        displacements[i] = displacements[i - 1] + colItem;
    }
    sendcounts[size - 1] = colItem;
    recvcounts[size - 1] = colItem;

    if (rank == 0) {
        printf("\n\n----------------------- \nrecvcounts and displacements \n");
        for (int i = 0; i < size; i++) {
            printf("\nsendcounts: [%d]: %d \n", i, sendcounts[i]);
            printf("recvcounts: [%d]: %d \n", i, recvcounts[i]);
            printf("displacements: [%d]: %d \n", i, displacements[i]);
        }
        printf("----------------------- \n");
    }


    //receive buffer init
    int** recvBuf = new int* [matrixLen];
    for (int i = 0; i < matrixLen; i++) {
        recvBuf[i] = new int[colItem];
    }


    for (int row = 0; row < matrixLen; row++) {
        MPI_Scatterv(matrix[row], sendcounts, displacements, MPI_INT, recvBuf[row], recvcounts[rank], MPI_INT, 0, MPI_COMM_WORLD);
    }

    int nextRank = rank + 1;
    if (rank == size - 1) nextRank = 0;

    int prevRank = rank - 1;
    if (rank == 0) prevRank = size - 1;

    printf("\n\n\nprocess %d: \n", rank);
    printf("my start coordinates [0, %d]: \n", displacements[rank]);
    printf("prev from me start coordinates [0, %d]: \n", displacements[prevRank]);
    printf("next to start coordinates [0, %d]: \n", displacements[nextRank]);

    printf("Values: \n");
    for (int i = 0; i < matrixLen; i++) {
        for (int j = 0; j < colItem; j++) {
            printf("[ %d ] ", recvBuf[i][j]);
        }
        printf("\n");
    }
    printf("\n\n\n");

    MPI_Finalize();
    return EXIT_SUCCESS;
}
