#include "EvolutionaryOptimizator.hpp"
#include "PerformanceEvaluator.hpp"
#include "Equation.hpp"
#include "Matrix.hpp"
#include "Microtime.hpp"
#include <algorithm>
#include <random>
#include <iterator>

EvolutionaryOptimizator::Organism::Organism(size_t rows, size_t cols)
    : field(rows, cols, true)
{
    
}

EvolutionaryOptimizator::Organism &EvolutionaryOptimizator::Organism::operator= (Organism const &other)
{
    #ifdef DEBUG
    if (field.Rows != other.field.Rows || field.Cols != other.field.Cols) throw new exception("Fields have different sizes");
    #endif
    loss = other.loss;
    for (size_t r = 0; r < field.Rows; r++)
        for (size_t c = 0; c < field.Cols; c++)
            field.Plane(r, c) = other.field.Plane(r, c);    
    return *this;
}

size_t EvolutionaryOptimizator::Organism::countPlanes() 
{
    size_t count = 0;
    for (size_t r = 0; r < field.Rows; r++)
        for (size_t c = 0; c < field.Cols; c++)
            if (field.Plane(r, c)) count++;
    return count;
}

EvolutionaryOptimizator::EvolutionaryOptimizator(const Support &supports, const vector<Force> &forces, const size_t organismsCount, const size_t orgRows, const size_t orgCols)
    : supports(supports), forces(forces), orgRows(orgRows), orgCols(orgCols)
    , currentGeneration(new vector<Organism>(organismsCount, Organism(orgRows, orgCols)))
    , nextGeneration(new vector<Organism>(organismsCount, Organism(orgRows, orgCols))) // TODO: Make sure, fields get deep copied
    , evaluator(orgRows, orgCols, supports, forces)
{

}

void EvolutionaryOptimizator::mutate(Organism &dest) 
{
    for (int i = 0; i < dest.field.Cols * dest.field.Rows / 10; i++) { // TODO: Introdouce exponentially decaying mutation rate
        // Chooses a random Col and a random row to execute the mutation
        int mutationCol = (rand() % (this->orgCols));
        int mutationRow = (rand() % (this->orgRows));

        // TODO: Do not alter supports etc.

        // Unset value only if there is an element as neighbour 
        if (!dest.field.Plane(mutationRow, mutationCol)) {
            if (dest.field.Plane(mutationRow, mutationCol - 1) || 
                    dest.field.Plane(mutationRow, mutationCol + 1) ||
                    dest.field.Plane(mutationRow - 1, mutationCol) ||
                    dest.field.Plane(mutationRow + 1, mutationCol)){

                dest.field.Plane(mutationRow, mutationCol) = true;
            }
        }
        else
        {
            // If there is an element: removed only if it's not in the middle of two neighbours or the only connection on an edge
            // 00000
            // 11110 <- do not remove this edge
            // 00010
            array<bool, 10> binaryValues = { dest.field.Plane(mutationRow - 1, mutationCol - 1),
                                            dest.field.Plane(mutationRow, mutationCol - 1),
                                            dest.field.Plane(mutationRow + 1, mutationCol - 1),
                                            dest.field.Plane(mutationRow + 1, mutationCol),
                                            dest.field.Plane(mutationRow + 1, mutationCol + 1),
                                            dest.field.Plane(mutationRow, mutationCol + 1),
                                            dest.field.Plane(mutationRow - 1, mutationCol + 1),
                                            dest.field.Plane(mutationRow - 1, mutationCol),
                                            dest.field.Plane(mutationRow - 1, mutationCol - 1)
                                            };
            int switchCount = 0;
            for (int i = 0; i < 9; i++)
                if (binaryValues[i] != binaryValues[i + 1]) switchCount++;
            // switchCount is ether 0, 2 or 4. Removing is fine, if it is not four
            dest.field.Plane(mutationRow, mutationCol) = false;
        }
    }
}

void EvolutionaryOptimizator::Evolve(size_t generations)
{
    for (size_t epoch = 0; epoch < generations; epoch++)
    {
        // Calculate loss for each organism
        for (Organism &org : *currentGeneration)
        {
            float stress = evaluator.GetPerformance(org.field, nullopt);
            if (stress > 10) // Mechanical structure broke, organism died
                org.loss = INFINITY;
            else
                org.loss = org.countPlanes();
        }

        // Sort accodring to loss
        sort(currentGeneration->begin(), currentGeneration->end(), [](Organism &a, Organism &b) {return a.loss < b.loss; });
        cout << (*currentGeneration)[0].loss << endl;

        // Setup a new organism, best 10% get children
        size_t childrenPerOrganism = currentGeneration->size() / 10;
        for (int i = 0; i < nextGeneration->size(); i++)
        {
            (*nextGeneration)[i] = (*currentGeneration)[i / childrenPerOrganism];
            mutate((*nextGeneration)[i]);
        }
        swap(currentGeneration, nextGeneration);
    }
}