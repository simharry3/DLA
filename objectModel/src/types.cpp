#include <types.h>

using namespace std;

Particle::Particle(int* position){
    for(int i = 0; i < 3; ++i){
        this->pos[i] = position[i];
    }
    this->mortonCode = 0;
    particleLock = false;
}

bool Particle::operator<(const Particle& p) const{
    return this->mortonCode < p.mortonCode;
}
bool Particle::operator<(const uint64_t v) const{
    return this->mortonCode < v;
}

void Particle::move(int* vec){
    for(int i = 0; i < 3; ++i){
        this->pos[i] = vec[i];
    }  
}

void Particle::encodeLocation(int* bounds){
    this->mortonCode = encodePosition(  (float)this->pos[0]/bounds[0], 
                                        (float)this->pos[1]/bounds[1], 
                                        (float)this->pos[2]/bounds[2]);
    
}


Universe::Universe(int size){
    srand(time(NULL));
    this->aggregatorLock = false;
    this->activeParticleLock = false;
    this->numParticles = 0;
    this->numAggregators = 0;
    for(int i = 0; i < 3; ++i){
        this->bounds[i] = size;
    }
    printf("SIMULATION BOUNDS: (%d, %d, %d)\n", 
                this->bounds[0], this->bounds[1], this->bounds[2]);
}

void Universe::addParticles(int number){
    this->numParticles = number;
    for(int i = 0; i < number; ++i){
        int location[3] = {rand()%this->bounds[0], rand()%this->bounds[1], rand()%this->bounds[2]};
        this->activeParticles.push_back(Particle(location));
    }
}

void Universe::addAggregators(char* filename){
    ifstream agFile;
    agFile.open(filename);
    if(!agFile.is_open()){
        fprintf(stderr, "ERROR OPENING AGGREGATOR FILE\n");
        exit(1);
    }
    int xi, yi, zi;
    while(agFile >> xi >> yi >> zi){
        int location[3] = {xi, yi, zi};
        this->aggregators.push_back(Particle(location));
        ++this->numAggregators;
    }
    agFile.close();
}

void Universe::generateAggregators(int num, int bound, int* center){
    while(this->aggregatorLock);
    this->aggregatorLock = true;
    for(int i = 0; i < num; ++i){
        int location[3] = {rand()%bound + center[0], rand()%bound + center[1], rand()%bound + center[2]};
        this->aggregators.push_back(Particle(location));
        ++this->numAggregators;
    }
    this->aggregatorLock = false;
}

void Universe::reserveMemory(){
    this->aggregators.reserve(this->numParticles + this->numAggregators);
}

void Universe::generateMortonCodes(){
    for(vector<Particle>::iterator i = this->aggregators.begin(); i != this->aggregators.end(); ++i){
        i->encodeLocation(this->bounds);
    }
    sort(this->aggregators.begin(), this->aggregators.end());
}

void Universe::moveParticles(){
    int vec[3];
    this->startingAggregators = this->numAggregators;
    while(this->aggregatorLock);
    this->aggregatorLock = true;
    this->activeParticleLock = true;
    list<Particle>::iterator i = this->activeParticles.begin();
    while(i != this->activeParticles.end()){
        for(int j = 0; j < 3; ++j){
            vec[j] = min(max(rand() % 3 - 1 + i->pos[j], 0), this->bounds[j]);
        }
        if(this->checkVacant(vec)){
            i->move(vec);
            ++i;
        }
        else{
            this->aggregators.push_back(*i);
            i = this->activeParticles.erase(i);
            --this->numParticles;
            ++this->numAggregators;
        }
    }
    this->aggregatorLock = false;
    this->activeParticleLock = false;
}

bool Universe::checkVacant(int* pos){
    Particle testParticle = Particle(pos);
    testParticle.encodeLocation(this->bounds);
    return(!binary_search(this->aggregators.begin(), this->aggregators.begin() + this->startingAggregators, testParticle));
}

void Universe::printParticles(){
    for(list<Particle>::iterator i = this->activeParticles.begin(); i != this->activeParticles.end(); ++i){
        int* temp = i->pos;
        printf("%d %d %d\n", temp[0], temp[1], temp[2]);
    }
}

void Universe::writeOutputFile(char* filename){
    ofstream outFile;
    outFile.open(filename);
    if(!outFile.is_open()){
        fprintf(stderr, "FAILED TO OPEN OUTPUT FILE\n");
        exit(1);
    }
    for(vector<Particle>::iterator i = this->aggregators.begin(); i != this->aggregators.end(); ++i){
        outFile << 0 << ' ' << i->pos[0] << ' ' << i->pos[1] << ' ' << i->pos[2] << '\n'; 
    }
    for(list<Particle>::iterator i = this->activeParticles.begin(); i != this->activeParticles.end(); ++i){
        outFile << 1 << ' ' << i->pos[0] << ' ' << i->pos[1] << ' ' << i->pos[2] << '\n'; 
    }
    outFile.close();
}

void Universe::renderUniverse(){

    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    int rc;
    rc = pthread_create(&(this->visualizerThread), NULL, runVisualizer, (void*)this);
    if(rc){
        fprintf(stderr, "Unable to start visualizer\n");
        exit(-1);
    }

    pthread_attr_destroy(&attr);
    // pthread_exit(NULL);
}