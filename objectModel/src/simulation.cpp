#include <types.h>
int numParticles = 1000;
int universeSize = 100;

int main(int argc, char* argv[]){
    Universe u1(universeSize);
    u1.addParticles(numParticles);
    u1.printParticles();
    // int step = 0;
    // while(step < 100){
    //     u1.moveParticles();
    //     ++step;
    //     printf("STEP %d | ACTIVE: %d | AGGREGATORS: %d\n", step, u1.numParticles, u1.numAggregators);
    //     // u1.printParticles();
    // }
    // u1.printParticles();
}