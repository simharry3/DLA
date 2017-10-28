/*
    OpenGL Visualizer for DLA simulation
*/
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