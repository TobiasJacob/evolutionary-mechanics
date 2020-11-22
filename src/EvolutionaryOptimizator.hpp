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

        typedef struct{

            //Initial Organisms, must be malloc'ed
            organism *organisms;
   
            //number of organisms in the array
            int organismsCount;

            //Tests organism's fitness to the problem
            unsigned int (*fitnessTest) (organism org);

            //Organigms lenght (?) not sure about this
            int orgRows;
            int orgCols;
            // int orgDnaLength;

            //How much we want to get to the perfect solution to our problem
            //The EA will converge here
            unsigned int desiredFitness;

            //Heads-up about how close the organisms are
            void(*progress) (unsigned int fitness);
        }evolutionary_algorithm;

        EvolutionaryOptimizator();
        EvolutionaryOptimizator(const Field &field, const Support &supports, const vector<Force> forces);
         //this evolves the group of organisms until they reach a satisfactory fitness level
        organism evolve(evolutionary_algorithm ea);

        //Reproduction using 2 organisms - crossing, then mutation
        organism reproduce(organism org1, organism org2, evolutionary_algorithm alg);

    private:
        const Support &supports;
        const vector<Force> forces;
        unsigned int fitnessTest(EvolutionaryOptimizator::organism orgToTest);

};

#endif

