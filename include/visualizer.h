////////////////////////////////////////////
//  FILE:   Visualizer.h
//  DESC:  Provides Particle and Universe
//          classes for DLA simulation.
//
//  AUTHOR: Clayton Rayment 
//  Date:   10/28/2017
////////////////////////////////////////////

#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <string>
#include <list>
#include <vector>
#include <fstream>
#include <algorithm>
#include <GL/glut.h>
#include "types.h"

//Call visualizer initializer used by pthreads
void* runVisualizer(void* args);

#endif