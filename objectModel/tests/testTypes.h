#include "types.h"

class ParticleTest{
    public:
        ParticleTest();
    private:
        Particle testParticle;

};

class UniverseTest{
    public:
        UniverseTest();
        bool testUniverseCreation();
        bool testUniverseDeletion();

    private:
        Universe testUniverse;
};