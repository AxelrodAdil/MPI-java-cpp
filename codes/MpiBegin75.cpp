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
    /*
    В каждом процессе дано вещественное число. Используя функцию MPI_Comm_split и одну коллективную операцию редукции, найти
максимальное из чисел, данных в процессах с четным рангом (включая
главный процесс), и минимальное из чисел, данных в процессах с нечетным рангом. 
Найденный максимум вывести в процессе 0, а найденный минимум — в процессе 1. 
    */

    int worldrank, worldsize;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &worldrank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldsize);

    int color = (worldrank % 2 == 0);
    float s = worldrank*37 + rand()%10;

    MPI_Comm newcomm;
    MPI_Comm_split(MPI_COMM_WORLD, color, worldrank, &newcomm);
    int newrank, newsize;
    MPI_Comm_rank(newcomm, &newrank);
    MPI_Comm_size(newcomm, &newsize);

    printf("[worldRank - %d] newRank %d: %f (color = %d)\n", worldrank, newrank, s, color);
    if (color == 0) {
        float min;
        MPI_Reduce(&s, &min, 1, MPI_FLOAT, MPI_MIN, 0, newcomm);
        if (newrank == 0)
            printf("min: %f on color - %d\n", min, color);
    }
    if (color == 1) {
        float max;
        MPI_Reduce(&s, &max, 1, MPI_FLOAT, MPI_MAX, 0, newcomm);
        if (newrank == 0)
            printf("max: %f on color - %d\n", max, color);
    }

    MPI_Finalize();
    return 0;
}
