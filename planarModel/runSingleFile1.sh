#!/bin/sh
#########################
#       SIZING:
#------------------------
#       SMALL: 64
#       MEDIUM: 512
#########################


#SBATCH -p small  
#SBATCH -O
#SBATCH -t 00:30:00
#################################
#CHANGE THESE EVERY RUN:

#################################
srun -n $1 ./simulation.xl $2 $3 $4 $5 $6 test1
