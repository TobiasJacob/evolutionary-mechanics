#!/bin/bash
#SBATCH --nodes=2
#SBATCH --ntasks=28
#SBATCH --cpus-per-task=1
#SBATCH --time=00:10:00
#SBATCH --job-name=hybrid
cd $SLURM_SUBMIT_DIR

# mpiifort -xHOST -O3 -qopenmp -mt_mpi hello_hybrid.f90 -o hello_hybrid.exe

# set variable so task placement works as expected
export I_MPI_JOB_RESPECT_PROCESS_PLACEMENT=0

mpirun -print-rank-map -n $SLURM_NTASKS -genv \
 OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK -genv I_MPI_PIN_DOMAIN=omp \
 build/program 50 150 10000 0.9995
