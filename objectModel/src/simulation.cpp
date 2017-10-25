#include "types.h"
#include "visualizer.h"
#include <unistd.h>
int numParticles = 1000;
int universeSize = 60;

int main(int argc, char* argv[]){
    Universe u1(universeSize);
    u1.addParticles(numParticles);
    int center[3] = {25, 25, 25};
    // u1.addAggregators((char*)"aggregators.dat");
    u1.generateAggregators(30, 5, center);
    initVisualizer(argc, argv);
    int step = 0;
    pthread_t visualizer;
    int rc;
    rc = pthread_create(&visualizer, NULL, runVisualizer, NULL);
    if(rc){
        fprintf(stderr, "Unable to start visualizer\n");
        exit(-1);
    }
    while(u1.numParticles > numParticles * .1){
        u1.moveParticles();
        ++step;
        printf("STEP %d | ACTIVE: %d | AGGREGATORS: %d\n", step, u1.numParticles, u1.numAggregators);
        u1.writeOutputFile((char*)"output.dat");
        // usleep(50000);
    }

    pthread_exit(NULL);
}