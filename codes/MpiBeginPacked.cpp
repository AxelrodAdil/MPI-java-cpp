#include "mpi.h"
#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char* argv[])
{
    //main init
    int size, rank, position, i;
    int b[10];
    float a[10];
    char buf[100];
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (int i = 0; i < 10; i++) {
        a[i] = rand()%50 + 1.0f;
        if (rank == 0) {
            b[i] = 1;
        }
        else {
            b[i] = 2;
        }
    }
    position = 0;

    if (rank == 0) {
        MPI_Pack(a, 10, MPI_FLOAT, buf, 100, &position, MPI_COMM_WORLD);
        MPI_Pack(b, 10, MPI_INT, buf, 100, &position, MPI_COMM_WORLD);
        MPI_Bcast(buf, 100, MPI_PACKED, 0, MPI_COMM_WORLD);
    }
    else {
        MPI_Bcast(buf, 100, MPI_PACKED, 0, MPI_COMM_WORLD);
        MPI_Unpack(buf, 100, &position, a, 10, MPI_FLOAT, MPI_COMM_WORLD);
        MPI_Unpack(buf, 100, &position, b, 10, MPI_INT, MPI_COMM_WORLD);
    }

    for (int i = 0; i < 4; i++) {
        printf("process id-%d : a=%f b=%d\n", rank, a[i], b[i]);
    }
    MPI_Finalize();
    return EXIT_SUCCESS;
}
