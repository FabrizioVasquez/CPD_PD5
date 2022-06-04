#!/bin/bash
#SBATCH --job-name=exe-pd
#SBATCH -o Tiempos_omp_mpi.out 

module load gcc/10.1.0
module load mpich/4.0

mpic++ -fopenmp omp_mpi_mvm.cpp -o omp_mpi.exe

for np in 4 8 16
do
 for nthreads in 4 8
    do
    mpirun -np $np ./omp_mpi.exe $nthreads
    done
done

module unload mpich/4.0
module unload gcc/10.1.0