#!/bin/sh
sbatch -o 1-2-256-8192.dat -N 1 runSingleFile1.sh 2 8192 256 50000
sbatch -o 1-4-256-8192.dat -N 1 runSingleFile1.sh 4 8192 256 50000
sbatch -o 1-8-256-8192.dat -N 1 runSingleFile1.sh 8 8192 256 50000
sbatch -o 1-16-256-8192.dat -N 1 runSingleFile1.sh 16 8192 256 50000
sbatch -o 1-32-256-8192.dat -N 1 runSingleFile1.sh 32 8192 256 50000
sbatch -o 2-64-256-8192.dat -N 2 runSingleFile1.sh 64 8192 256 50000
sbatch -o 4-128-256-8192.dat -N 4 runSingleFile1.sh 128 8192 256 50000
sbatch -o 8-256-256-8192.dat -N 8 runSingleFile1.sh 256 8192 256 50000
#gooool sbatch -o 16-512-1024-8192.dat -N 16 runSingleFile1.sh 512 8192 256 50000
#gooool sbatch -o 32-1024-1024-8192.dat -N 32 runSingleFile1.sh 1024 8192 256 50000
