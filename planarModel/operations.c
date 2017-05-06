#include "operations.h"
#include <mpi.h>

//=========================================================================
//Wrapper function to run one step of the system. This gets called n times
//per simulation:
//=========================================================================
void stepSystem(state* st, context* ctx){
    // printf("STEP %u\n", st->simSteps);
    // fflush(NULL);
    updateParticlePositions(st, ctx);

}

//==============================================
//Main wrapper function to run the MPI system. 
// This gets called once per simulation:
//==============================================
void runSystem(state* st, context* ctx){
    int cont = 1;
    int temp = 1;
    MPI_Barrier(MPI_COMM_WORLD);
    //printf("%d\n", st->activeParticles);
    //Check to see if we are running using a set endpoint:
    if(ctx->numSteps > 0){
    	int i;
        for(i = 0; i < ctx->numSteps; ++i){
            //  printf("STEPS: %d\n", st->simSteps);
            // fflush(NULL);
            stepSystem(st, ctx);
            ++st->simSteps;
        }
    }  //Otherwise run until there are no active particles:
    else{
        MPI_Request request, request2;
        while(cont > 0){
            stepSystem(st, ctx);
            ++st->simSteps;

            MPI_Barrier(MPI_COMM_WORLD);
            cont = 0;
            if(st->activeParticles > 0){
                temp = 1;
            }
            else{
                temp = 0;
            }

            //This issue took a while to debug. Turned out we were not calling recv
            //as often as we were calling send, so we were getting very strange 
            //memory corruption. MAKE SURE YOU CALL BOTH EQUALLY
            for(int i = 0; i < ctx->comm_size; i++){
                MPI_Isend(&temp, 1, MPI_INT, i, 234, MPI_COMM_WORLD, &request2);
            }
            MPI_Barrier(MPI_COMM_WORLD);
            int tval = 0;
            for(int i = 0; i < ctx->comm_size; i++){
                MPI_Irecv(&cont, 1, MPI_INT, i, 234, MPI_COMM_WORLD, &request);
                tval += cont;
            }
            cont = tval;
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
}


//============================================
//Wrapper function for pretty output that is
//formatted correctly for our pyplot program
//============================================
void printSimulationResults(state* st, context* ctx){
        if(ctx->rank == 0){
            printf("%u %f\n", st->simSteps, st->simTime);
            fflush(NULL);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        printState(st, ctx);
        MPI_Barrier(MPI_COMM_WORLD);
}