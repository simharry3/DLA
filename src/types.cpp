#include "types.h"

using namespace std;

Particle::Particle(int* position){
    setPosition(position);
    setMortonCode(0);
}

bool Particle::operator<(const Particle& p) const{
    return this->getMortonCode() < p.getMortonCode();
}
bool Particle::operator<(const uint64_t v) const{
    return this->getMortonCode() < v;
}

void Particle::setPosition(int* loc){
    for(int i = 0; i < 3; ++i){
        position_[i] = loc[i];
    }  
}

void Particle::encodeLocation(const int* bounds){
    const int* pos = getPosition();
    mortonCode_ = encodePosition((float)pos[0]/bounds[0], 
                                (float)pos[1]/bounds[1], 
                                (float)pos[2]/bounds[2]);
}

//////////////////////////////////////////////////////////

Universe::Universe(int size){
    srand(time(NULL));
    aggregatorLock_ = false;
    activeParticleLock_ = false;
    numActiveParticles_ = 0;
    numAggregators_ = 0;
    for(int i = 0; i < 3; ++i){
        bounds_[i] = size;
    }
    printf("SIMULATION BOUNDS: (%d, %d, %d)\n", 
                bounds_[0], bounds_[1], bounds_[2]);
}

Universe::~Universe(){
    pthread_join(visualizerThread_, NULL);
    pthread_exit(NULL);
}

void Universe::addParticles(int number){
    numActiveParticles_ = number;
    for(int i = 0; i < number; ++i){
        int location[3] = {rand()%bounds_[0], rand()%bounds_[1], rand()%bounds_[2]};
        activeParticles_.push_back(Particle(location));
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
        xi = max(min(xi, bounds_[0]), 0);
        yi = max(min(yi, bounds_[1]), 0);
        zi = max(min(zi, bounds_[2]), 0);
        int location[3] = {xi, yi, zi};
        aggregators_.push_back(Particle(location));
        ++numAggregators_;
    }
    agFile.close();
}

void Universe::generateAggregators(int num, int bound, int* center){
    this->lockAggregators();
    for(int i = 0; i < num; ++i){
        int location[3] = {rand()%bound + center[0], rand()%bound + center[1], rand()%bound + center[2]};
        aggregators_.push_back(Particle(location));
        ++numAggregators_;
    }
    this->releaseAggregatorLock();
}

void Universe::reserveMemory(){
    aggregators_.reserve(numActiveParticles_ + numAggregators_);
}

void Universe::generateMortonCodes(){
    for(vector<Particle>::iterator i = aggregators_.begin(); i != aggregators_.end(); ++i){
        i->encodeLocation(bounds_);
    }
    // this->lockAggregators();
    sort(aggregators_.begin(), aggregators_.end());
    // this->releaseAggregatorLock();
}

void Universe::moveParticles(){
    int vec[3];
    startingAggregators_ = numAggregators_;
    // this->lockActiveParticles();
    // this->lockAggregators();
    const int* pos;
    list<Particle>::iterator i = activeParticles_.begin();
    while(i != activeParticles_.end()){
        pos = i->getPosition();
        for(int j = 0; j < 3; ++j){           
            vec[j] = min(max(rand() % 3 - 1 + pos[j], 0), bounds_[j]);
        }
        if(checkVacant(vec)){
            i->setPosition(vec);
            ++i;
        }
        else{
            aggregators_.push_back(*i);
            i = activeParticles_.erase(i);
            --numActiveParticles_;
            ++numAggregators_;
        }
    }
    // this->releaseActiveParticleLock();
    // this->releaseAggregatorLock();
}

bool Universe::checkVacant(int* pos){
    Particle testParticle = Particle(pos);
    testParticle.encodeLocation(bounds_);
    return(!binary_search(aggregators_.begin(), aggregators_.begin() + startingAggregators_, testParticle));
}

void Universe::printParticles(){
    for(list<Particle>::iterator i = activeParticles_.begin(); i != activeParticles_.end(); ++i){
        const int* pos = i->getPosition();
        printf("%d %d %d\n", pos[0], pos[1], pos[2]);
    }
}

void Universe::writeOutputFile(char* filename){
    ofstream outFile;
    outFile.open(filename);
    if(!outFile.is_open()){
        fprintf(stderr, "FAILED TO OPEN OUTPUT FILE\n");
        exit(1);
    }
    const int* pos;
    for(vector<Particle>::iterator i = aggregators_.begin(); i != aggregators_.end(); ++i){
        pos = i->getPosition();
        outFile << 0 << ' ' << pos[0] << ' ' << pos[1] << ' ' << pos[2] << '\n'; 
    }
    for(list<Particle>::iterator i = activeParticles_.begin(); i != activeParticles_.end(); ++i){
        pos = i->getPosition();
        outFile << 1 << ' ' << pos[0] << ' ' << pos[1] << ' ' << pos[2] << '\n'; 
    }
    outFile.close();
}

list<vector<int> >* Universe::generateOutputList(){
    list<vector<int> >* outputList = new list<vector<int> >;
    this->lockActiveParticles();
    this->lockAggregators();
    const int* pos;
    for(int i = 0; i < this->getNumAggregators(); ++i){
        pos = aggregators_[i].getPosition();
        vector<int> tempVec;
        tempVec.push_back(pos[0]);
        tempVec.push_back(pos[1]);
        tempVec.push_back(pos[2]);
        tempVec.push_back(0);
        outputList->push_back(tempVec);
    }
    for(list<Particle>::iterator i = activeParticles_.begin(); i != activeParticles_.end(); ++i){
        pos = i->getPosition();
        vector<int> tempVec;
        tempVec.push_back(pos[0]);
        tempVec.push_back(pos[1]);
        tempVec.push_back(pos[2]);
        tempVec.push_back(1);
        outputList->push_back(tempVec);
    }
    this->releaseActiveParticleLock();
    this->releaseAggregatorLock();
    return(outputList);
}

void Universe::renderUniverse(){

    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    int rc;
    rc = pthread_create(&(visualizerThread_), NULL, runVisualizer, (void*)this);
    if(rc){
        fprintf(stderr, "Unable to start visualizer\n");
        exit(-1);
    }

    pthread_attr_destroy(&attr);
    // pthread_exit(NULL);
}