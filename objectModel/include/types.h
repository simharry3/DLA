////////////////////////////////////////////
//  FILE:   Types.h
//  DESC:  Provides Particle and Universe
//          classes for DLA simulation.
//
//  AUTHOR: Clayton Rayment 
//  Date:   10/28/2017
////////////////////////////////////////////

#ifndef TYPES_H
#define TYPES_H

#include "utilities.h"

#include <stdint.h>
#include <pthread.h>

#include <list>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

class Particle{
    public:
        Particle(int*);

        /*
            Operator overload for Particle < Particle.
            Operation compares via Morton codes.
        */
        bool operator<(const Particle&) const;

        /*
            Operator overload for Particle < uint64_t.
            Operation assumes input value is a valid morton code.
        */
        bool operator<(const uint64_t) const;
        
        /*
            Encodes the Particle's position with a 30-bit Morton code.
        */
        void encodeLocation(const int*);

        /*
            Gets Particle's morton code
        */
        uint64_t getMortonCode() const {return mortonCode_;}

        /*
            Gets Particle's position
        */
        const int* getPosition() const {return position_;}

        /*
            Sets Particle's morton code
        */
        void setMortonCode(uint64_t mortonCode){mortonCode_ = mortonCode;}

        /*
            Sets Particle's Position
        */
        void setPosition(int*);

    private:        
        int position_[3];

        uint64_t mortonCode_;

        bool particleLock;
};

class Universe{
    public:
        Universe(int size);
        ~Universe();

        void addParticles(int);
        void addAggregators(char*);
        void generateAggregators(int, int, int*);
        void reserveMemory();
        void generateMortonCodes();
        void moveParticles();
        void checkCollisions();
        bool checkVacant(int*);
        void printParticles();
        void writeOutputFile(char*);
        void renderUniverse();

        std::list<std::vector<int> >* generateOutputList();

        //GETTERS:
        const int* getBounds() const {return bounds_;}
        int getNumActiveParticles() const {return numActiveParticles_;}
        int getNumAggregators() const {return numAggregators_;}

        void lockAggregators(){while(aggregatorLock_); aggregatorLock_ = true;}
        void lockActiveParticles(){while(activeParticleLock_); activeParticleLock_ = true;}

        void releaseAggregatorLock(){aggregatorLock_ = false;}
        void releaseActiveParticleLock(){activeParticleLock_ = false;}


    private:
        int bounds_[3];

        bool aggregatorLock_;
        bool activeParticleLock_;

        int numActiveParticles_;
        int numAggregators_;
        int startingAggregators_;

        //Use lists to enable parallelization later, as container mondification is safe
        std::list<Particle> activeParticles_;

        //Use vector for aggregators, since deletions do not occur, and so we can binary search
        std::vector<Particle> aggregators_;        

        pthread_t visualizerThread_;
};

//Forward declaration of the visualizer thread
void* runVisualizer(void*);
#endif