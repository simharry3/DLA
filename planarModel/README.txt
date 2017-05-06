Philip LaGambino and Clayton Rayment
Project ReadMe

This readMe is for anyone using the Diffustion Limited Aggregation project for parallel computing. 
the parallelPlane iteration.

Compiling on a laptop:
all: types.h types.c operations.h operations.c simulation.c 
    mpicc -I. -Wall -O3 *.c -o simulation

Arguements (on a laptop):
mpirun -np <number of ranks> simulation <number of particles> <world size> <number of steps (-1 for full run through)> > <aggregator file>

Compiling on AMOS:
all: types.h types.c operations.h operations.c simulation.c 
    mpixlc -I. -Wall -O3 *.c -o simulation.xl

Arguements (on a laptop):
srun -np <number of ranks> simulation.xl <number of particles> <world size> <number of steps (-1 for full run through)>  <aggregator file>

STRUCT INFORMATION:


typedef struct context{
    uint numParticles; - number of particles overall
    uint particlesPerRank; - number of particles per rank
    int comm_size; - the number of MPI_Ranks
    int rank; - the MPI_Rank id
    int numSteps; - number of overall steps taken (used for a -1 timestep)
    int chkFreq; - the checkpoint frequency command to print to the checkpointFile
    int humanOutput; - the human output flag

    int* max; - the size informatino of the universe
    char* checkpointFile; - the chekpoint file name (unused)
}context;

typedef struct state{
    uint activeParticles; - number of active particle for this MPI_Rank
    uint collidedParticles; - number of collided particle for this MPI_Rank
    uint simSteps; - the current system step

    int** universe; - Univere is an array of flattened matrix. In other words the second array is actually a matrix representing the x,z plane. If the matrix is of 4x4 then the corresponding array would be of size 16.

    //ADD STATE
}state;

OTHER INFORMATION: THIS IS THE SIMULATION WE WOULD WANT YOU TO RUN FOR OUR PARALLEL TESTING