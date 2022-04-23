#include "mpi.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
int main(int argc, char* argv[])
{
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int rootRank = 0;

    int length = size + 5;
    int* sendBuf = new int[length];
    int* recvBuf = new int[length];

    printf("\n Proccess: [%d], [Dlina massiva: %d] \n\n", rank, length);
    for (int i = 0; i < length; i++) {
        srand((length + i) * (i + rank));
        sendBuf[i] = rand() % 100;
    }
    for (int i = 0; i < length; i++) {
        printf("%d process: [%d number: %d] \n", rank, i, sendBuf[i]);
    }

    MPI_Reduce(sendBuf, recvBuf, length, MPI_INT, MPI_SUM, rootRank, MPI_COMM_WORLD);
    if (rank == rootRank) {
        printf("\n\nSumma %d: \n\n", rank);
        for (int i = 0; i < length; i++){
            printf(" %d: %d |", i, recvBuf[i]);
        }
        printf("\n");
    }
    MPI_Finalize();
    return EXIT_SUCCESS;
}
