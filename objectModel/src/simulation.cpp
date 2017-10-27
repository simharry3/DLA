#include "types.h"
#include "visualizer.h"
#include <unistd.h>
int numParticles = 1000;
int universeSize = 600;

int main(int argc, char* argv[]){
    Universe* u1 = new Universe(atoi(argv[1]));
    u1->addParticles(atoi(argv[2]));
    int center[3] = {3, 3, 3};
    // u1.addAggregators((char*)"aggregators.dat");
    u1->generateAggregators(1, 1, center);
    center[0] = u1->bounds[0] - 3;
    center[1] = u1->bounds[1] - 3;
    center[2] = u1->bounds[2] - 3;
    u1->generateAggregators(1, 1, center);
    u1->reserveMemory();

    u1->renderUniverse();
    
    int step = 0;
    while(u1->numParticles > atoi(argv[2]) * .1){
        u1->generateMortonCodes();
        u1->moveParticles();
        ++step;
        // printf("STEP %d | ACTIVE: %d | AGGREGATORS: %d\n", step, u1->numParticles, u1->numAggregators);
        // u1.writeOutputFile((char*)"output.dat");
        // sleep(1);
    }
    pthread_join(u1->visualizerThread, NULL);
    pthread_exit(NULL);
}