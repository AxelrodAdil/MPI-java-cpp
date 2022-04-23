#include "mpi.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

int main(int argc, char* argv[])
{
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

    //init options
    int* sendcounts = new int[size];
    int* recvcounts = new int[size];
    int* displacements = new int[size];

    displacements[0] = 0;
    for (int i = 1; i <= size - 1; i++) 
    {
        sendcounts[i - 1] = eachProccItemLen;
        recvcounts[i - 1] = eachProccItemLen;
        displacements[i] = displacements[i - 1] + eachProccItemLen;
    }
    sendcounts[size - 1] = eachProccItemLen;
    recvcounts[size - 1] = eachProccItemLen;

    if (rank == 0) {
        printf("\n\n----------------------- \nrecvcounts and displacements \n");
        for (int i = 0; i < size; i++) {
            printf("\nsendcounts: [%d]: %d \n", i, sendcounts[i]);
            printf("recvcounts: [%d]: %d \n", i, recvcounts[i]);
            printf("displacements: [%d]: %d \n", i, displacements[i]);
        }
        printf("----------------------- \n");
    }

    int* recvBuf = new int[eachProccItemLen];

    MPI_Scatterv(matrix, sendcounts, displacements, MPI_INT, recvBuf, recvcounts[rank], MPI_INT, 0, MPI_COMM_WORLD);

    int nextRank = rank + 1;
    if (rank == size - 1) nextRank = 0;

    int prevRank = rank - 1;
    if (rank == 0) prevRank = size - 1;

    printf("\n\n\nprocess %d: \n", rank);
    printf("my start coordinates [0, %d]: \n", displacements[rank] / matrixLen);
    printf("up from me start coordinates [0, %d]: \n", displacements[prevRank] / matrixLen);
    printf("down to start coordinates [0, %d]: \n", displacements[nextRank] / matrixLen);

    printf("Values: \n");
    for (int i = 0; i < eachProccItemLen; i++)
    {
        printf("[ %d ] ", recvBuf[i]);
        if (i == matrixLen - 1) printf("\n");
    }
    printf("\n\n\n");

    MPI_Finalize();
    return EXIT_SUCCESS;
}
