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

        /*
            Randomly generate a specified number of particles within the system bounds
        */
        void addParticles(int);

        /*
            Add aggregator particles from file
        */
        void addAggregators(char*);
        
        /*
            Randomly generate a specified number of aggregator particles within a given radius
        */
        void generateAggregators(int, int, int*);

        /*
            Reserve the memory of the aggregator vector to avoid resizing
        */
        void reserveMemory();
        
        /*
            Generate morton codes for all aggregator particles and sort the vector
        */
        void generateMortonCodes();


        /*
            Check to see if a given position is vacant using morton codes and binary search
        */
        bool checkVacant(int*);

        /*
            Move particles and call to check collisions between particles
        */
        void moveParticles();

        /*
            Print output info for all particles in the universe
        */
        void printParticles();

        /*
            Write all particle into to file, one particle per line
        */
        void writeOutputFile(char*);

        /*
            Pthread management function, launches visualizer in new thread
        */
        void renderUniverse();

        /*
            Generates the output list required by the visualizer so no direct access is required
        */
        std::list<std::vector<int> >* generateOutputList();

        /*
            Get the bounds of the universe
        */
        const int* getBounds() const {return bounds_;}

        /*
            Get the number of active particles
        */
        int getNumActiveParticles() const {return numActiveParticles_;}
        
        /*
            Get the number of aggregator particles
        */
        int getNumAggregators() const {return numAggregators_;}

        /*
            Engage the lock for the aggregators vector to avoid iterator fouling across threads
        */
        void lockAggregators(){while(aggregatorLock_); aggregatorLock_ = true;}
       
        /*
            Engage the lock for the active particle list to avoid iterator fouling across threads
        */
        void lockActiveParticles(){while(activeParticleLock_); activeParticleLock_ = true;}


        /*
            Release locks
        */
        void releaseAggregatorLock(){aggregatorLock_ = false;}
        void releaseActiveParticleLock(){activeParticleLock_ = false;}


    private:
        int bounds_[3];

        bool aggregatorLock_;
        bool activeParticleLock_;

        int numActiveParticles_;
        int numAggregators_;
        int startingAggregators_;

       std::list<Particle> activeParticles_;

        //Use vector for aggregators, since deletions do not occur, and so we can binary search
        std::vector<Particle> aggregators_;        

        pthread_t visualizerThread_;
};

//Forward declaration of the visualizer thread
void* runVisualizer(void*);
#endif