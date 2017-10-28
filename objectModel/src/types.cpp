#include "types.h"

using namespace std;


//PARTICLE://////////////////////////////////////////////////////////////////
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
    setMortonCode(encodePosition((float)pos[0]/bounds[0], 
                                (float)pos[1]/bounds[1], 
                                (float)pos[2]/bounds[2]));
}
//////////////////////////////////////////////////////////////////////////////


//UNIVERSE:///////////////////////////////////////////////////////////////////
Universe::Universe(int size){
    srand(time(NULL));
    this->aggregatorLock_ = false;
    this->activeParticleLock_ = false;
    this->numActiveParticles_ = 0;
    this->numAggregators_ = 0;
    for(int i = 0; i < 3; ++i){
        this->bounds_[i] = size;
    }
    printf("SIMULATION BOUNDS: (%d, %d, %d)\n", 
                this->bounds_[0], this->bounds_[1], this->bounds_[2]);
}

Universe::~Universe(){
    pthread_join(this->visualizerThread_, NULL);
    pthread_exit(NULL);
}

void Universe::addParticles(int number){
    this->numActiveParticles_ = number;
    for(int i = 0; i < number; ++i){
        int location[3] = {rand()%this->bounds_[0], rand()%this->bounds_[1], rand()%this->bounds_[2]};
        this->activeParticles_.push_back(Particle(location));
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
        this->aggregators_.push_back(Particle(location));
        ++this->numAggregators_;
    }
    agFile.close();
}

void Universe::generateAggregators(int num, int bound, int* center){
    this->lockAggregators();
    for(int i = 0; i < num; ++i){
        int location[3] = {rand()%bound + center[0], rand()%bound + center[1], rand()%bound + center[2]};
        this->aggregators_.push_back(Particle(location));
        ++this->numAggregators_;
    }
    this->releaseAggregatorLock();
}

void Universe::reserveMemory(){
    this->aggregators_.reserve(this->numActiveParticles_ + this->numAggregators_);
}

void Universe::generateMortonCodes(){
    for(vector<Particle>::iterator i = this->aggregators_.begin(); i != this->aggregators_.end(); ++i){
        i->encodeLocation(this->bounds_);
    }
    sort(this->aggregators_.begin(), this->aggregators_.end());
}

void Universe::moveParticles(){
    int vec[3];
    this->startingAggregators_ = this->numAggregators_;
    this->lockActiveParticles();
    this->lockAggregators();
    const int* pos;
    list<Particle>::iterator i = this->activeParticles_.begin();
    while(i != this->activeParticles_.end()){
        pos = i->getPosition();
        for(int j = 0; j < 3; ++j){           
            vec[j] = min(max(rand() % 3 - 1 + pos[j], 0), this->bounds_[j]);
        }
        if(this->checkVacant(vec)){
            i->setPosition(vec);
            ++i;
        }
        else{
            this->aggregators_.push_back(*i);
            i = this->activeParticles_.erase(i);
            --this->numActiveParticles_;
            ++this->numAggregators_;
        }
    }
    this->releaseActiveParticleLock();
    this->releaseAggregatorLock();
}

bool Universe::checkVacant(int* pos){
    Particle testParticle = Particle(pos);
    testParticle.encodeLocation(this->bounds_);
    return(!binary_search(this->aggregators_.begin(), this->aggregators_.begin() + this->startingAggregators_, testParticle));
}

void Universe::printParticles(){
    for(list<Particle>::iterator i = this->activeParticles_.begin(); i != this->activeParticles_.end(); ++i){
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
    for(vector<Particle>::iterator i = this->aggregators_.begin(); i != this->aggregators_.end(); ++i){
        pos = i->getPosition();
        outFile << 0 << ' ' << pos[0] << ' ' << pos[1] << ' ' << pos[2] << '\n'; 
    }
    for(list<Particle>::iterator i = this->activeParticles_.begin(); i != this->activeParticles_.end(); ++i){
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
        pos = this->aggregators_[i].getPosition();
        vector<int> tempVec;
        tempVec.push_back(pos[0]);
        tempVec.push_back(pos[1]);
        tempVec.push_back(pos[2]);
        tempVec.push_back(0);
        outputList->push_back(tempVec);
    }
    for(list<Particle>::iterator i = this->activeParticles_.begin(); i != this->activeParticles_.end(); ++i){
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
    rc = pthread_create(&(this->visualizerThread_), NULL, runVisualizer, (void*)this);
    if(rc){
        fprintf(stderr, "Unable to start visualizer\n");
        exit(-1);
    }

    pthread_attr_destroy(&attr);
    // pthread_exit(NULL);
}