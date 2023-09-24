#!/bin/sh

#SBATCH --account=courses0101
#SBATCH --partition=debug
#SBATCH --ntasks=1
#SBATCH --ntasks-per-node=4
#SBATCH --cpus-per-task=4
#SBATCH --time=00:01:00

module load gcc
gcc -fopenmp parallel.c -o output
export OMP_NUM_THREADS=16 

srun ./output