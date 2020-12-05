#include "EvolutionaryOptimizator.hpp"
#include "PerformanceEvaluator.hpp"
#include "Equation.hpp"
#include "Matrix.hpp"
#include "Microtime.hpp"
#include <algorithm>
#include <random>
#include <iterator>

EvolutionaryOptimizator::Organism::Organism(size_t rows, size_t cols)
    : field(new Field(rows, cols, true))
{
    
}

EvolutionaryOptimizator::Organism::Organism(Organism const &other)
    : loss(other.loss), field(new Field(other.field->Rows, other.field->Cols))
{
    for (size_t r = 0; r < field->Rows; r++)
        for (size_t c = 0; c < field->Cols; c++)
            field->Plane(r, c) = other.field->Plane(r, c);    
}

EvolutionaryOptimizator::Organism &EvolutionaryOptimizator::Organism::operator= (Organism const &other)
{
    #ifdef DEBUG
    if (field->Rows != other.field->Rows || field->Cols != other.field->Cols) throw "Fields have different sizes";
    #endif
    loss = other.loss;
    for (size_t r = 0; r < field->Rows; r++)
        for (size_t c = 0; c < field->Cols; c++)
            field->Plane(r, c) = other.field->Plane(r, c);    
    return *this;
}

EvolutionaryOptimizator::Organism::Organism(Organism &&other) 
{
    field = move(other.field);
    loss = other.loss;
}

EvolutionaryOptimizator::Organism& EvolutionaryOptimizator::Organism::operator= (Organism &&other) 
{
    if (this != &other) 
    {
        field = move(other.field);
        loss = other.loss;
    }
    return *this;
}

size_t EvolutionaryOptimizator::Organism::countPlanes() 
{
    size_t count = 0;
    for (size_t r = 0; r < field->Rows; r++)
        for (size_t c = 0; c < field->Cols; c++)
            if (field->Plane(r, c)) count++;
    return count;
}

EvolutionaryOptimizator::EvolutionaryOptimizator(const Support &supports, const vector<Force> &forces, const size_t organismsCount, const size_t orgRows, const size_t orgCols)
    : supports(supports), forces(forces), orgRows(orgRows), orgCols(orgCols)
    , evaluator(orgRows, orgCols, supports, forces)
    , currentGeneration(new vector<Organism>(organismsCount, Organism(orgRows, orgCols)))
    , nextGeneration(new vector<Organism>(organismsCount, Organism(orgRows, orgCols)))
{

}

void EvolutionaryOptimizator::mutate(Organism &dest, size_t alteratedFields) 
{
    size_t alterations = 0;
    while (alterations < alteratedFields) {
        // Chooses a random Col and a random row to execute the mutation
        size_t mutationCol = rand() % orgCols;
        size_t mutationRow = rand() % orgRows;

        auto protectedAccess = [&](size_t r, size_t c)
        {
            if (r < dest.field->Rows && c < dest.field->Cols) 
                return dest.field->Plane(r, c); 
            return false; // Structures out of view count as not set
        };

        // Unset value only if there is an element as neighbour 
        if (!dest.field->Plane(mutationRow, mutationCol))
        {
            if (protectedAccess(mutationRow, mutationCol - 1) || 
                    protectedAccess(mutationRow, mutationCol + 1) ||
                    protectedAccess(mutationRow - 1, mutationCol) ||
                    protectedAccess(mutationRow + 1, mutationCol)) {
                dest.field->Plane(mutationRow, mutationCol) = true;
                alterations++;
            }
        }
        else
        {
            bool isSupport = false;
            for (const Point &p : supports.ColSupports) if ((p.col == mutationCol || p.col == mutationCol + 1) && (p.row == mutationRow || p.row == mutationRow + 1)) {isSupport = true; break;}
            for (const Point &p : supports.RowSupports) if ((p.col == mutationCol || p.col == mutationCol + 1) && (p.row == mutationRow || p.row == mutationRow + 1)) {isSupport = true; break;}
            if (isSupport) continue;

            bool isOnForce = false;
            for (const Force &f : forces) if ((f.attackCorner.col == mutationCol || f.attackCorner.col == mutationCol + 1) && (f.attackCorner.row == mutationRow || f.attackCorner.row == mutationRow + 1)) {isSupport = true; break;}
            if (isOnForce) continue;
            
            // If there is an element: removed only if it's not in the middle of two neighbours or the only connection on an edge
            // And if it is not at a support
            // 00000
            // 11110 <- do not remove this edge
            // 00010
            array<bool, 10> binaryValues = { protectedAccess(mutationRow - 1, mutationCol - 1),
                                            protectedAccess(mutationRow, mutationCol - 1),
                                            protectedAccess(mutationRow + 1, mutationCol - 1),
                                            protectedAccess(mutationRow + 1, mutationCol),
                                            protectedAccess(mutationRow + 1, mutationCol + 1),
                                            protectedAccess(mutationRow, mutationCol + 1),
                                            protectedAccess(mutationRow - 1, mutationCol + 1),
                                            protectedAccess(mutationRow - 1, mutationCol),
                                            protectedAccess(mutationRow - 1, mutationCol - 1)
                                            };
            size_t switchCount = 0;
            for (size_t i = 0; i < 9; i++)
                if (binaryValues[i] != binaryValues[i + 1]) switchCount++;
            // switchCount is ether 0, 2 or 4. Removing is fine, if it is not four
            if (switchCount < 4)
            {
                dest.field->Plane(mutationRow, mutationCol) = false;
                alterations++;
            }
        }
    }
}

void EvolutionaryOptimizator::Evolve(const size_t generations, const float maxStress, const float alterationDecay)
{
    float alterations = orgRows * orgCols / 10;
    for (size_t epoch = 0; epoch < generations; epoch++)
    {
        // Calculate loss for each organism
        for (size_t i = 0; i < currentGeneration->size(); i++)
        {
            Organism &org = (*currentGeneration)[i];

            optional<string> debugSave = nullopt;
            if (i == 0 && epoch % 10 == 0)
                debugSave = string("debug/Debug-") + to_string(epoch) + ".html";
            
            float stress = evaluator.GetPerformance(*org.field, debugSave);
            if (stress > maxStress) // Mechanical structure broke, organism died
                org.loss = INFINITY;
            else
                org.loss = org.countPlanes();
        }

        // Sort according to loss
        sort(currentGeneration->begin(), currentGeneration->end(), [](Organism &a, Organism &b) {return a.loss < b.loss; });
        cout << epoch << " ALT:" << alterations << " Planes: " << (*currentGeneration)[0].loss << endl;

        // Setup a new organism, best 10% get children
        size_t childrenPerOrganism = currentGeneration->size() / 10;
        for (size_t i = 0; i < nextGeneration->size(); i++)
        {
            (*nextGeneration)[i] = (*currentGeneration)[i / childrenPerOrganism];
            mutate((*nextGeneration)[i], alterations);
        }
        swap(currentGeneration, nextGeneration);
        alterations *= alterationDecay;
    }
}