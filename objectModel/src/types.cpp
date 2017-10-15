#include <types.h>

Particle::Particle(int* position){
    for(int i = 0; i < 3; ++i){
        this->pos[i] = position[i];
    }
}

void Particle::move(int* vec){
    for(int i = 0; i < 3; ++i){
        this->pos[i] += vec[i];
    }  
}


Universe::Universe(int size){
    for(int i = 0; i < 3; ++i){
        this->bounds[i] = size;
    }
    printf("SIMULATION BOUNDS: (%d, %d, %d)\n", 
                this->bounds[0], this->bounds[1], this->bounds[2]);
}

void Universe::addParticles(int number){
    this->particles = number;
    this->activeParticles = (Particle*)malloc(number * sizeof(Particle));
    for(int i = 0; i < number; ++i){
        int location[3] = {rand()%this->bounds[0], rand()%this->bounds[1], rand()%this->bounds[2]};
        this->activeParticles[i] = Particle(location);
    }
}

void Universe::moveParticles(){
    int vec[3];
    for(int i = 0; i < this->particles; ++i){
        for(int i = 0; i < 3; ++i){
            vec[i] = rand() % 3 - 1;
        }
        if(this->checkVacant(this->particles[i].pos)
            this->activeParticles[i].move(vec);
        }
        else{
            //ADD TO COLLIDED PARTICLES LIST
        }
    }
}

void Universe::printParticles(){
    for(int i = 0; i < this->particles; ++i){
        int* temp = this->activeParticles[i].pos;
        printf("(%d, %d, %d)\n", temp[0], temp[1], temp[2]);
    }
}