#include <iostream>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

    int size, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int nextRank = rank + 1;
    if (rank == size - 1) nextRank = 0;

    int prevRank = rank - 1;
    if (rank == 0) prevRank = size - 1;

    srand(rank);
    int data = rand() % 100;
    std::cout << data << "\n";

    MPI_Send(&data, 1, MPI_INT, nextRank, 0, MPI_COMM_WORLD);
    MPI_Recv(&data, 1, MPI_INT, prevRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Process %d received number %d from process %d\n", rank, data, prevRank);


    /*else {
        MPI_Recv(&data, 1, MPI_INT, prevRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(&data, 1, MPI_INT, nextRank, 0, MPI_COMM_WORLD);
        printf("Process %d received number %d from process %d\n", rank, data, prevRank);
    }*/

    MPI_Finalize();
}
