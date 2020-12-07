# Evolutionary Mechanics

Following the [Project Description](latex/sources/ProjectDescription.md)

## Instructions
In order to run in Bridges, due to the dependency on C++17, make sure you run `module load gcc/9.2.0` first.

- The latest, distributed, parallelized and most optimized version is contained on the root folder, and the follow steps must be followed:
    1. Make sure you have the MPI module available using `mpi/gcc/openmpi-1.6.4` using `module add mpi/openmpi-x86_64`
    2. `make`
    3. `mpirun -np X build/program <N> <Organisms> <Epochs> <Decay>` (recommend 20 100 1000 0.995f)
            
- The sequential implementations and the basic parallelized implementation are all contained in their own folder. Please, use the following steps:
    1. `cd version_to_execute`
    2. `make`
    3. `build/program <N> <Organisms> <Epochs> <Decay>` (recommend 20 100 1000 0.995f)
    
## Directory Structure

```batch
/evolutionary_mechanics
|-- /basic_parallel_implementation
|-- /basic_sequential_implementation
|-- /best_optimized_sequential_implementation
|-- /jobFiles (jobs submitted)
|-- /slurm (contains slurm outputs)
|-- /src (contains the source code of the best optimized parallel implementation)
|-- report.pdf
|-- /latex (source code of our reports)
|   |
|   |-- /description
|   |-- /performance
|   |-- /progressReport
|   |-- /report
|   |-- /sources
```
