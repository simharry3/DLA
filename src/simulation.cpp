#include "types.h"
#include <stdlib.h>
#include <time.h>
// #include <unistd.h>

int main(int argc, char* argv[]){
    if(argc < 3){
        printf("USAGE: <dir>/bin/simulation <Size> <Num Particles> <aggregator.txt(optional)>\n");
        return EXIT_FAILURE;
    }
    Universe* u1 = new Universe(atoi(argv[1]));
    u1->addParticles(atoi(argv[2]));
    if(argc == 3){
        int center[3] = {3, 3, 3};
        u1->generateAggregators(1, 1, center);
        const int* bounds = u1->getBounds();
        center[0] = bounds[0] - 3;
        center[1] = bounds[1] - 3;
        center[2] = bounds[2] - 3;
        u1->generateAggregators(1, 1, center);
    }
   else if(argc > 3){
        u1->addAggregators(argv[3]);
   }
    u1->reserveMemory();

    u1->renderUniverse();
    
    int step = 0;
    while(u1->getNumActiveParticles() > atoi(argv[2]) * .1){
        u1->generateMortonCodes();
        u1->moveParticles();
        ++step;
    }
    delete u1;
    return EXIT_SUCCESS;
}