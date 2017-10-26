#ifndef TYPES_H
#define TYPES_H

#include <iostream>
#include <fstream>

#include <pthread.h>

#include "visualizer.h"

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
        void addAggregators(char*);
        void generateAggregators(int, int, int*);

        void moveParticles();
        void checkCollisions();
        bool checkVacant(int*);

        void printParticles();

        void writeOutputFile(char* filename);


        void renderUniverse();

        int bounds[3];

        int numParticles;
        int numAggregators;

        //Use lists to enable parallelization later, as container mondification is safe
        list<Particle> activeParticles;
        list<Particle> aggregators;
};
#endif