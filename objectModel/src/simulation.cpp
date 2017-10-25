#include "types.h"
#include "visualizer.h"
#include <unistd.h>
int numParticles = 1000;
int universeSize = 100;

int main(int argc, char* argv[]){
    Universe u1(universeSize);
    u1.addParticles(numParticles);
    u1.addAggregators((char*)"aggregators.dat");
    int step = 0;
    // while(u1.numParticles > numParticles * .1){
    //     u1.moveParticles();
    //     ++step;
    //     printf("STEP %d | ACTIVE: %d | AGGREGATORS: %d\n", step, u1.numParticles, u1.numAggregators);
    //     // u1.writeOutputFile((char*)"output.dat");
    //     // sleep(1);
    // }

    // u1.writeOutputFile((char*)"output.dat");
    runVisualizer(argc, argv);
    // u1.printParticles();
}