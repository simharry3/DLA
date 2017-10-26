#include "types.h"
#include "visualizer.h"
#include <unistd.h>
int numParticles = 1000;
int universeSize = 600;

int main(int argc, char* argv[]){
    Universe u1(atoi(argv[1]));
    u1.addParticles(atoi(argv[2]));
    int center[3] = {u1.bounds[0]/2, u1.bounds[1]/2, u1.bounds[2]/2};
    printf("%s\n", argv[2]);
    // u1.addAggregators((char*)"aggregators.dat");
    u1.generateAggregators(30, 5, center);
    initVisualizer(argc, argv);
    u1.renderUniverse();
    
    int step = 0;
    while(u1.numParticles > atoi(argv[2]) * .1){
        u1.moveParticles();
        ++step;
        // printf("STEP %d | ACTIVE: %d | AGGREGATORS: %d\n", step, u1.numParticles, u1.numAggregators);
        // u1.writeOutputFile((char*)"output.dat");
        // usleep(50000);
    }
}