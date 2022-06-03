#!/bin/bash
#SBATCH --job-name=exe-pd
#SBATCH -o Tiempos_mpi.out 
#BATCH --nodes=1 --ntasks=1 --cpus-per-task=2 

module load gcc/10.1.0
module load mpich/4.0

for i in 2 4 8 16
do
 mpirun -np $i ./a.out
done

module unload mpich/4.0
module unload gcc/10.1.0