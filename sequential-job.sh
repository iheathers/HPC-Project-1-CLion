#!/bin/sh

#SBATCH --account=courses0101
#SBATCH --partition=debug
#SBATCH --ntasks=1
#SBATCH --time=00:01:00

module load gcc

gcc -fopenmp sequential.c -o experiment -lm

srun ./experiment