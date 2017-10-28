#ifndef TYPES_H
#define TYPES_H

#include "utilities.h"

#include <fstream>

#include <pthread.h>

#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <list>
#include <vector>

class Particle{
    public:
        Particle(int*);

        //Operator overloads:
        bool operator<(const Particle&) const;
        bool operator<(const uint64_t) const;

        void encodeLocation(const int*);

        //Getters:
        uint64_t getMortonCode() const {return mortonCode_;}
        const int* getPosition() const {return position_;}

        //Setters:
        void setMortonCode(uint64_t mortonCode){mortonCode_ = mortonCode;}
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

        std::list<std::vector<int> >* generateOutputList();

        void renderUniverse();

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