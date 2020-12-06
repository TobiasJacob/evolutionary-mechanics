#ifndef EVOLUTIONARYOPTIMIZATOR
#define EVOLUTIONARYOPTIMIZATOR

#include <vector>
#include <optional>
#include <mpi.h>
#include "Matrix.hpp"
#include "Field.hpp"
#include "Equation.hpp"
#include "PerformanceEvaluator.hpp"
#include "Organism.hpp"

class EvolutionaryOptimizator
{
    private:
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

