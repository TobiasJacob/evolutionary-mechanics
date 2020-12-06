/**
 * @file EvolutionaryOptimizator.hpp
 * @author Tobias Jacob - Galliera Raffaele - Ali Muddasar
 * @brief 
 * @version 1.0
 * @date 2020-12-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */
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

/**
 * @brief The Evolutionary Optimizator follows a Genetic Evolutionary approach for finding the most performing Organisms starting from a base population.
 It consists of two stages, one takes care of choosing the best Organisms and the second to apply random mutations to this best Organisms. The number of
 alterations applied will decay, the more the evolution progresses with the number of generations the less the mutation will be applied to the organisms.
 * 
 */
class EvolutionaryOptimizator
{
    private:
        const Support &supports; // Supports of the structural problem 
        const vector<Force> &forces; // Forces applied to the structural problem
        const size_t orgRows; // Number of Rows of the organisms
        const size_t orgCols; // Number of Cols of the organisms
        PerformanceEvaluator evaluator; // Performance Evaluator for the organisms

        unique_ptr<vector<Organism> > globalGeneration; // Global generation representing the current state of our population

        /**
         * @brief Following the Evolutionary Approach, the mutate function is going to alter the dest field executing alteratedFields mutations.
         Every mutation choose a random col and random row where to execute the mutation the mutation is going to be effective just if it generates a
         valid, connected structure.

         * 
         * @param dest Destination Organisms
         * @param alteratedFields Number field to be alterated
         */
        void mutate(Organism &dest, size_t alteratedFields);
    public:
        /**
         * @brief Construct a new Evolutionary Optimizator object
         * 
         */
        EvolutionaryOptimizator();
        /**
         * @brief Construct a new Evolutionary Optimizator object
         * 
         * @param supports 
         * @param forces 
         * @param organismsCount 
         * @param orgRows 
         * @param orgCols 
         */
        EvolutionaryOptimizator(const Support &supports, const vector<Force> &forces, const size_t organismsCount, const size_t orgRows, const size_t orgCols);
        
        /**
         * @brief The evolve function is the core function of the Evolutionary Approach, it takes as an input the number of generations we want to 
         create starting from a starting population of Organisms. The function will take of mutating all the Organisms contained in the current Generation
         and calculate their perfomance. A number of best Organisms will be chosen and set as the next Generation to evolve.
         
         MPI Optimization: 
         Every node will take care of mutating and calculating the perfomance for a group of Organisms. The main node will gather all the informations and
         sort them according to their perfomance. The a group of best Organisms will be broadcasted to all the ndoes restarting the process.

         * 
         * @param generations 
         * @param maxStress 
         * @param alterationDecay 
         */
        void Evolve(const size_t generations, const float maxStress, const float alterationDecay);
};

#endif

