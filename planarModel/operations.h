#ifndef _OPERATIONS_H_
#define _OPERATIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "types.h"
#include <mpi.h>

void runSystem(state* st, context* ctx);
void stepSystem(state* st, context* ctx);
void printSimulationResults(state* st, context* ctx);

#endif /*_OPERATIONS_H_*/