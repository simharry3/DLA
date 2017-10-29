////////////////////////////////////////////
//  FILE:   Utilities.h
//  BRIEF:  Provides utility functions for 
//          DLA simulation.
//
//  AUTHOR: Clayton Rayment 
//  Date:   10/28/2017
////////////////////////////////////////////
#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdint.h>
#include <algorithm>

/*
    Returns the expanded input value.
    EX: 1100101 -> 100100000000100000100
    Taken from: https://devblogs.nvidia.com/parallelforall/thinking-parallel-part-iii-tree-construction-gpu/
*/
uint32_t expandBits(unsigned int);


/*
    Encodes the [0,1] unit cube input location to a 30-bit Morton code.
    Taken from: https://devblogs.nvidia.com/parallelforall/thinking-parallel-part-iii-tree-construction-gpu/
*/
uint64_t encodePosition(float, float, float);

#endif