#include <iostream>
#include <time.h>
#include <string.h>

using namespace std;

class Particle{
    public:
        Particle(int* position);

        void move(int* vec);
        int pos[3];
};

class Universe{
    public:
        Universe(int size);

        void addParticles(int number);
    
        void moveParticles();
        void checkCollisions();

        void printParticles();

        void writeOutputFile(string filename);

        int bounds[3];

        int particles;
        Particle* activeParticles;
        Particle* collidedParticles;
};