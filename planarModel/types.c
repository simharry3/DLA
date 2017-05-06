#include <types.h>
#include <mpi.h>


//===========================================
//  This function initializes the particle in
//  the grid structure
//===========================================
void initParticle(state* st, context* ctx, int* pos, int value){
    st->universe[pos[2]][pos[1] * ctx->max[0] + pos[0]] = value;
}

//===========================================
//  This function checks to see if the movement
//  of a particle will cause it to collide
//===========================================
int checkParticleCollision(state* st, context* ctx, int* pos){
    return  st->universe[pos[2]][pos[1] * ctx->max[0] + pos[0]];
}

//===========================================
//  This funciton caps the position of a particle
//  so that it does not escape our simulation bounds
//===========================================
void capPosition(context* ctx, int** pos){
    if((*pos)[0] > ctx->max[0] - 1){
        (*pos)[0] = ctx->max[0] - 1;
    }
    else if((*pos)[0] < 0){
        (*pos)[0] = 0;
    }

    if((*pos)[1] > ctx->max[1] - 1){
        (*pos)[1] = ctx->max[1] - 1;
    }
    else if((*pos)[1] < 0){
        (*pos)[1] = 0;
    }

    if((*pos)[2] > ctx->planesPerRank - 1){
        (*pos)[2] = ctx->planesPerRank - 1;
    }
    else if((*pos)[2] < 0){
        (*pos)[2] = 0;
    }
}

//===========================================
//  our simulation is limited on the sides, 
//  however can wrap around on the top and
//  bottom for ease of programming. This function
//  performs the calculation for wrapping the rank
//  should it be at the top or bottom of our simulation
//===========================================
int wrapRank(state* st, context* ctx, int rankIn){
    if(rankIn > ctx->comm_size - 1){
        return 0;
    }
    if(rankIn < 0){
        return ctx->comm_size - 1;
    }
    return rankIn;
}

//===========================================
//  This is the function that updates the ghost
//  rows in the simulation. 
//===========================================
void updateGhostRows(state* st, context* ctx){
    MPI_Request send1, send2;
    MPI_Request recv1, recv2;
    MPI_Status status1, status2;
    size_t plane = ctx->max[0] * ctx->max[1];
    int* buff1 = calloc(plane, sizeof(int));
    int* buff2 = calloc(plane, sizeof(int));

    // printf("RANK %d FORWARD RANK: %d BACKWARD RANK: %d \n",
    //         ctx->rank, wrapRank(st, ctx, ctx->rank  + 1), wrapRank(st, ctx, ctx->rank - 1));
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Isend((st->universe[0]), plane, 
            MPI_INT, wrapRank(st, ctx, ctx->rank  - 1), 123, MPI_COMM_WORLD, &send1);
    MPI_Isend((st->universe[ctx->planesPerRank + 1]), plane, 
            MPI_INT, wrapRank(st, ctx, ctx->rank  + 1), 124, MPI_COMM_WORLD, &send2);
    MPI_Barrier(MPI_COMM_WORLD);
    //Another issue that took a while to debug... We weren't blocking on recv
    //so access to buffx would occationally occur before it was ready.
    //MAKE SURE YOU ARE DONE READING TO BUFFER BEFORE USE.
    MPI_Recv(buff1, plane, 
            MPI_INT, wrapRank(st, ctx, ctx->rank  - 1), 124, MPI_COMM_WORLD, &status1);
    MPI_Recv(buff2, plane, 
            MPI_INT, wrapRank(st, ctx, ctx->rank  + 1), 123, MPI_COMM_WORLD, &status2);
    MPI_Barrier(MPI_COMM_WORLD);
    // printf("=============================\n");
    for(int i = 0; i < ctx->max[0] * ctx->max[1]; ++i){
        // printf("%d ", buff1[i]);
        if(st->universe[1][i] >= EMPTY_CELL){
            st->universe[1][i] += buff1[i];
        }
        if(st->universe[ctx->planesPerRank][i] >= EMPTY_CELL){
            st->universe[ctx->planesPerRank][i] += buff2[i];
        }

        st->universe[0][i] = EMPTY_CELL;
        st->universe[ctx->planesPerRank + 1][i] = EMPTY_CELL;
    }
    
    //Not freeing these was causing a HUGE memory leak:
    free(buff1);
    free(buff2);
}


//===========================================
//  Main workhorse of the simulation. Scans through
//  the entire plane, moving each particle it encounters
//  that isn't an aggregator or collided particle
//===========================================
void updateParticlePositions(state* st, context* ctx){
    int num;
    int* d = calloc(3, sizeof(int));
    d[0] = d[1] = d[2] = 0;
    int** moveList = NULL;
    int movedParticles = 0;
    int* pos = calloc(3, sizeof(int));

   updateGhostRows(st, ctx);

    for(int i = 1; i < ctx->planesPerRank + 1; ++i){
        for(int j = 0; j < ctx->max[0] * ctx->max[1]; ++j){
            if(st->universe[i][j] >= ACTIVE_PARTICLE && st->universe[i][j] < ACTIVE_PARTICLE + CELL_MAX){
                pos[0] = j % ctx->max[0];
                pos[1] =  (j - pos[0]) / ctx->max[1];
                pos[2] = i;
                if(ctx->rank == 0){
                    // printf("%d %d %d %d\n", pos[0], pos[1], pos[2], st->universe[pos[2]][pos[1] * ctx->max[0] + pos[0]]);
                    fflush(NULL);
                }
                int particlesOnTile = st->universe[i][j] - (ACTIVE_PARTICLE - 1);
                st->universe[i][j] = EMPTY_CELL;
                for(int g = 0; g < particlesOnTile; ++g){
                    num = rand() % 6;
                    switch(num){
                        case 0:
                            d[0] = -1;
                            break;
                        case 1:
                            d[0] = 1;
                            break;
                        case 2:
                            d[1] = -1;
                            break;
                        case 3:
                            d[1] = 1;
                            break;
                        case 4:
                            d[2] = -1;
                            break;
                        case 5:
                            d[2] = 1;
                            break;
                    }
                    pos[0] += d[0];
                    pos[1] += d[1];
                    pos[2] += d[2];
                    capPosition(ctx, &pos);
                    int checkedValue = checkParticleCollision(st, ctx, pos);
                    if(checkedValue < EMPTY_CELL){
                        st->universe[i][j] = COLLIDED_PARTICLE;
                        // printf("HERE<<<<<<<<<<<<<<<<<<<<<<<\n");
                        fflush(NULL);
                    }
                    else if(checkedValue >= EMPTY_CELL && checkedValue < CELL_MAX){
                        moveList = realloc(moveList, (movedParticles + 1) * sizeof(int*));
                        moveList[movedParticles] = calloc(4, sizeof(int));

                        for(int k = 0; k < 3; ++k){
                            moveList[movedParticles][k] = pos[k];
                        }

                        ++movedParticles;
                    }
                    else{
                        --j;
                    }
                }
            }
        }
    }

    st->activeParticles = movedParticles;
    for(int i = 0; i < movedParticles; ++i){
        if(st->universe[moveList[i][2]][moveList[i][1] * ctx->max[0] + moveList[i][0]] >= EMPTY_CELL){
            st->universe[moveList[i][2]][moveList[i][1] * ctx->max[0] + moveList[i][0]] += 1;
        }
        free(moveList[i]);
    }
    free(moveList);
    free(pos);
    free(d);
    if(ctx->rank == 0){
        //printf("====== %u =======\n", st->activeParticles);
        fflush(NULL);
    }
    // sleep(1);
}

//===========================================
//  This function initializes the context struct
//===========================================
void initContext(context** ctx, int* data){
    *ctx = calloc(1, sizeof(context));

    MPI_Comm_size(MPI_COMM_WORLD, &((*ctx)->comm_size));
    MPI_Comm_rank(MPI_COMM_WORLD, &((*ctx)->rank));

    (*ctx)->max = calloc(3, sizeof(int));
    (*ctx)->numParticles = data[0];
    (*ctx)->max[0] = (*ctx)->max[1] = (*ctx)->max[2] = data[1];
    (*ctx)->numSteps = data[2];
    

    (*ctx)->planesPerRank = (*ctx)->max[2]/(*ctx)->comm_size;
    (*ctx)->particlesPerRank = (*ctx)->numParticles/(*ctx)->comm_size;
    
}


//===========================================
//  This function initializes the state struct
//=========================================== 
void initState(state** st, context* ctx){
    *st = calloc(1, sizeof(state));
    int* pos = (int*)calloc(3, sizeof(int));

    //We use a double pointer that is a representation of planes of our simulation. This is a compromise for 
    //Memory conservation's sake:
    size_t plane = ctx->max[0] * ctx->max[1];
    (*st)->universe = malloc((ctx->planesPerRank + 2) * sizeof(int*));
    for(int i = 0; i < ctx->planesPerRank + 2; ++i){
        (*st)->universe[i] = calloc(plane, sizeof(int));
        memset((*st)->universe[i], EMPTY_CELL, plane * sizeof(int));
    }

    int i;

    for(i = 0; i < ctx->particlesPerRank; ++i){
        pos[0] = rand() % ctx->max[0];
        pos[1] = rand() % ctx->max[1];
        pos[2] = (rand() % ctx->planesPerRank) + 1;
        // printf("ID: %d, CHECKING %d %d %d\n", i, pos[0], pos[1], pos[2]);
        // fflush(NULL);
        int checkVal = (*st)->universe[pos[2]][pos[1] * ctx->max[0] + pos[0]];
        if(checkVal >= EMPTY_CELL){
            // printf("ID: %d, PLACING PARTICLE\n", i);
            // fflush(NULL);
                initParticle(*st, ctx, pos, checkVal + 1);
        }
        else{
            // printf("ID: %d, OVERLAP\n", i);
            // fflush(NULL);
            --i;
        }
    }

    (*st)->activeParticles = ctx->particlesPerRank;
    //  printf("PPR: %u\n", (*st)->activeParticles);
    (*st)->collidedParticles = 0;
    (*st)->simSteps = 0;
    st = NULL;
    free(pos);
    //printState(*st, ctx);
}



//===========================================
//  This function destroys the state struct
//===========================================
void destroyState(state** st, context* ctx){
    for(int i = 0; i < ctx->planesPerRank + 2; ++i){
        free((*st)->universe[i]);
    }

   free((*st)->universe);
   free((*st));
   st = NULL;
}



//===========================================
//  This function initializes the aggregator
//  particles on the grid
//===========================================
void initAggregators(state* st, context* ctx, char* agFile){
    
    // if(my_rank == 0)
    //     printf("READING AG DATA FROM FILE: %s\n", agFile);
    
    int* pos = (int*)calloc(3, sizeof(int));
    int* buff = (int*)calloc(8, sizeof(int));
    FILE* fp;
    fp = fopen(agFile, "r");
    while(fscanf(fp, "%d %d %d", &pos[0], &pos[1], &pos[2]) != EOF){
        if(ctx->rank == pos[2]/ctx->planesPerRank){
            initParticle(st, ctx, pos, AGGREGATOR_PARTICLE);
        }
        // ++st->collidedParticles;
    }
    fclose(fp);
    fp = NULL;
    free(pos);
    free(buff);

    //printf("READING AGGREGATORS \n");
}




//===========================================
//  This function outputs pretty values that
//  are formatted correctly for pyplot to read
//===========================================
void printState(state* st, context* ctx){
   int* pos = calloc(3, sizeof(int));
    for(int i = 1; i < ctx->planesPerRank + 1; ++i){
        for(int j = 0; j < ctx->max[0] * ctx->max[1]; ++j){
            if(st->universe[i][j] < EMPTY_CELL){
                pos[0] = j % ctx->max[0];
                pos[1] =  (j - pos[0]) / ctx->max[1];
                pos[2] = (ctx->rank / ctx->planesPerRank) + i;
                printf("%d %d %d %d\n", pos[0], pos[1], pos[2], st->universe[i][j]);
                fflush(NULL);
            }
        }
    }
    free(pos);
}