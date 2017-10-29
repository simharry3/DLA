#include "types.h"
#include <stdlib.h>
#include <time.h>
// #include <unistd.h>
int numParticles = 1000;
int universeSize = 600;

int main(int argc, char* argv[]){
    Universe* u1 = new Universe(atoi(argv[1]));
    u1->addParticles(atoi(argv[2]));
    int center[3] = {3, 3, 3};
    // u1.addAggregators((char*)"aggregators.dat");
    u1->generateAggregators(1, 1, center);
    const int* bounds = u1->getBounds();
    center[0] = bounds[0] - 3;
    center[1] = bounds[1] - 3;
    center[2] = bounds[2] - 3;
    u1->generateAggregators(1, 1, center);
    u1->reserveMemory();

    u1->renderUniverse();
    
    int step = 0;
    while(u1->getNumActiveParticles() > atoi(argv[2]) * .1){
        // clock_t start = clock();
        u1->generateMortonCodes();
        u1->moveParticles();
        ++step;
        // clock_t end = clock();
        // printf("%fms\n", (double)(end-start)*1000.0/CLOCKS_PER_SEC);
        // printf("STEP %d | ACTIVE: %d | AGGREGATORS: %d\n", step, u1->numParticles, u1->numAggregators);
        // u1.writeOutputFile((char*)"output.dat");
        // sleep(1);
    }
    delete u1;
}