#ifndef EVOLUTIONARYOPTIMIZATOR
#define EVOLUTIONARYOPTIMIZATOR

#include <vector>
#include <optional>
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
        struct Organism {
            float loss = 0.0;
            unique_ptr<Field> field;
            Organism(size_t rows, size_t cols);
            // Organism is copyable
            Organism(Organism const &other);
            Organism &operator= (Organism const &other);
            Organism(Organism &&other);
            Organism &operator= (Organism &&other);
            // Move able too
            size_t countPlanes();
        };

        const Support &supports;
        const vector<Force> &forces;
        const size_t orgRows;
        const size_t orgCols;
        PerformanceEvaluator evaluator;

        unique_ptr<vector<Organism> > currentGeneration;
        unique_ptr<vector<Organism> > nextGeneration;
        
        void mutate(Organism &dest, size_t alteratedFields);
    public:
        EvolutionaryOptimizator();
        EvolutionaryOptimizator(const Support &supports, const vector<Force> &forces, const size_t organismsCount, const size_t orgRows, const size_t orgCols);
        
        void Evolve(const size_t generations, const float maxStress, const float alterationDecay);
};

#endif
