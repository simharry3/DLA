#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>

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
        bool checkVacant(int*);

        void printParticles();

        void writeOutputFile(string filename);

        int bounds[3];

        int numParticles;
        int numAggregators;

        //Use lists to enable parallelization later, as container mondification is safe
        list<Particle> activeParticles;
        list<Particle> aggregators;
};