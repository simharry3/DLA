#include <types.h>

Particle::Particle(int* position){
    for(int i = 0; i < 3; ++i){
        this->pos[i] = position[i];
    }
}

void Particle::move(int* vec){
    for(int i = 0; i < 3; ++i){
        this->pos[i] = vec[i];
    }  
}


Universe::Universe(int size){
    srand(time(NULL));
    for(int i = 0; i < 3; ++i){
        this->bounds[i] = size;
    }
    printf("SIMULATION BOUNDS: (%d, %d, %d)\n", 
                this->bounds[0], this->bounds[1], this->bounds[2]);
}

void Universe::addParticles(int number){
    this->numParticles = number;
    this->numAggregators = 0;
    for(int i = 0; i < number; ++i){
        int location[3] = {rand()%this->bounds[0], rand()%this->bounds[1], rand()%this->bounds[2]};
        this->activeParticles.push_back(Particle(location));
    }
}

void Universe::moveParticles(){
    int vec[3];
    for(list<Particle>::iterator i = this->activeParticles.begin(); i != this->activeParticles.end(); ++i){
        for(int j = 0; j < 3; ++j){
            vec[j] = min(max(rand() % 3 - 1 + i->pos[j], 0), this->bounds[j]);
        }
        if(this->checkVacant(vec)){
            i->move(vec);
        }
        else{
            this->aggregators.push_back(*i);
            this->activeParticles.erase(i);
            --this->numParticles;
            ++this->numAggregators;
            --i;
        }
    }
}

bool Universe::checkVacant(int* pos){
    for(list<Particle>::iterator i = this->aggregators.begin(); i != this->aggregators.end(); ++i){
        if(i->pos[0] == pos[0] && i->pos[1] == pos[1] && i->pos[2] == pos[2]){
            return false;
        }
    }
    return true;
}

void Universe::printParticles(){
    for(list<Particle>::iterator i = this->activeParticles.begin(); i != this->activeParticles.end(); ++i){
        int* temp = i->pos;
        printf("%d %d %d\n", temp[0], temp[1], temp[2]);
    }
}