#ifndef EVOLUTIONARY_OPTIMIZATOR_HPP
#define EVOLUTIONARY_OPTIMIZATOR_HPP

#include "Matrix.hpp"
#include "Field.hpp"
#include "Equation.hpp"
#include "PerformanceEvaluator.hpp"

#define MATE_VARIANCE_CONSTANT 0.3
#define MUTATIONS_PER_BASE_PAIR_CONSTANT 0.1

class EvolutionaryOptimizator
{   
    public:
        typedef struct{
            Field* field;
            int rows, cols;
        }organism;

        EvolutionaryOptimizator();
        EvolutionaryOptimizator(const Field &field, const Support &supports, const vector<Force> forces);
         //this evolves the group of organisms until they reach a satisfactory fitness level
        organism evolve();

        //Reproduction using 2 organisms - crossing, then mutation
        organism reproduce(organism org1, organism org2);

    private:
        const Support &supports;
        const vector<Force> forces;
        void copyOrganism(organism org1, organism org2, int rows, int cols, int startingRow, int startingCol);
        void simpleCrossingOver(organism org1, organism org2, organism dest, int rows, int cols, int startingRow, int startingCol);
        //Initial Organisms, must be malloc'ed
        organism *organisms;
   
        //number of organisms in the array
        int organismsCount;

        //Tests organism's fitness to the problem
        unsigned int (*fitnessTest) (organism org);

        //Organigms lenght (?) not sure about this
        int orgRows;
        int orgCols;

        //How much we want to get to the perfect solution to our problem
        //The EA will converge here
        unsigned int desiredFitness;

        //Heads-up about how close the organisms are
        void(*progress) (unsigned int fitness);
};

#endif

