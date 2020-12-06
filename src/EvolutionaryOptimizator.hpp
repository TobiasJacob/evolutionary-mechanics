#ifndef EVOLUTIONARYOPTIMIZATOR
#define EVOLUTIONARYOPTIMIZATOR

#include <vector>
#include <optional>
#include <mpi.h>
#include "Matrix.hpp"
#include "Field.hpp"
#include "Equation.hpp"
#include "PerformanceEvaluator.hpp"

#define MATE_VARIANCE_CONSTANT 0.3
#define MUTATIONS_PER_BASE_PAIR_CONSTANT 0.1
#define MAX_NUMBER_OF_REPRODUCTIONS 10

class EvolutionaryOptimizator
{
    private:
        struct Organism { // float loss, size_t row, size_t cols, bool[row * col] dim
            static unique_ptr<MPI_Datatype> mpiDatatype;
            float loss = 0.0;
            unique_ptr<Field> field;
            Organism(size_t rows, size_t cols);
            // Organism is copyable
            Organism(Organism const &other);
            Organism &operator= (Organism const &other);
            Organism(Organism &&other);
            Organism &operator= (Organism &&other);
            size_t countPlanes();
            static MPI_Datatype &getDatatype(size_t rows, size_t cols);
            static size_t getSize(size_t rows, size_t cols);
            void writeIntoBuffer(void *buffer);
            void readFromBuffer(void *buffer);
        };

        const Support &supports;
        const vector<Force> &forces;
        const size_t orgRows;
        const size_t orgCols;
        PerformanceEvaluator evaluator;

        unique_ptr<Organism> currentOrganism;
        unique_ptr<Organism> nextOrganism;
        
        unique_ptr<vector<Organism>> currentGeneration;

        void mutate(Organism &dest, size_t alteratedFields);
    public:
        EvolutionaryOptimizator();
        EvolutionaryOptimizator(const Support &supports, const vector<Force> &forces, const size_t organismsCount, const size_t orgRows, const size_t orgCols);
        
        void Evolve(const size_t generations, const float maxStress, const float alterationDecay);
};

#endif

