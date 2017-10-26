#ifndef TYPES_H
#define TYPES_H

#include <iostream>
#include <fstream>

#include <pthread.h>
#include <unistd.h>

#include "visualizer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>

using namespace std;

class Particle{
    public:
        Particle(int* position);
        bool operator<(const Particle&) const;

        void encodeLocation(int*);

        void move(int* vec);
        int pos[3];
        uint64_t mortonCode;
};

class Universe{
    public:
        Universe(int size);

        void addParticles(int number);
        void addAggregators(char*);
        void generateAggregators(int, int, int*);

        void generateMortonCodes();

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

        pthread_t visualizerThread;
};
#endif