#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <stddef.h>
#include "types.h"
#include "operations.h"

int main(int argc, char* argv[]){
    
    state* st;
    context* ctx;

    int mpi_rank;
    int mpi_commsize;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    //<NUM PARTICLES> <SIZE> <NUM STEPS> <chkFreq> <humanOutput> <inputFile>
    double start, end;
    start = MPI_Wtime();
    int* data = calloc(argc - 1, sizeof(int));
    int i;
    for(i = 1; i < argc; ++i){
        data[i - 1] = strtol(argv[i], NULL, 10);
    }

    //Random Variable Initialization
    srand((unsigned) time(NULL));
    
    initContext(&ctx, data);
    initState(&st, ctx);
    initAggregators(st, ctx, argv[argc-1]);
    // // if(mpi_rank == 0)
    // //     printState(st, ctx);
    runSystem(st, ctx);
    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();
    st->simTime = end - start;
    printSimulationResults(st, ctx);
    sleep(10);
    MPI_Barrier(MPI_COMM_WORLD);
    destroyState(&st, ctx);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    exit(0);
}

