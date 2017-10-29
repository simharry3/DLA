This software runs a diffusion limited aggregation until 90% 
of the starting particles have been aggregated.

Currently the aggregation is only placed in two corners of the simulation
bounds at the start of the simulation, however there is support for input file reading.


Usage:
after building simply run:

<dir>/bin/simulation <Size> <Particles>

The visualizer currently works best with a simulation size of anything less than 100, however this will be
fixed in the future.

TODO:
-Add lighting to "Fancy" render mode
-Add input aggregator flags