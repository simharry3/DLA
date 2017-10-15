#include <types.h>

int main(int argc, char* argv[]){
    Universe u1(256);
    u1.addParticles(4);
    u1.printParticles();
    u1.moveParticles();
    u1.printParticles();
}