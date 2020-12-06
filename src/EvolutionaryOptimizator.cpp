#include "EvolutionaryOptimizator.hpp"
#include "PerformanceEvaluator.hpp"
#include "Equation.hpp"
#include "Matrix.hpp"
#include "Microtime.hpp"
#include <algorithm>
#include <random>
#include <iterator>

EvolutionaryOptimizator::EvolutionaryOptimizator(const Support &supports, const vector<Force> &forces, const size_t organismsCount, const size_t orgRows, const size_t orgCols)
    : supports(supports), forces(forces), orgRows(orgRows), orgCols(orgCols)
    , evaluator(orgRows, orgCols, supports, forces)
    , currentGeneration(new vector<Organism>(organismsCount, Organism(orgRows, orgCols)))
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
            for (const Force &f : forces) if ((f.attackCorner.col == mutationCol || f.attackCorner.col == mutationCol + 1) && (f.attackCorner.row == mutationRow || f.attackCorner.row == mutationRow + 1)) {isOnForce = true; break;}
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
    int rank;
    double time;
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    size_t organismsCount = currentGeneration->size();
    
    size_t orgSize = Organism::getSize(this->orgCols, this->orgRows);
    vector<byte> singleBuffer(orgSize);
    vector<byte> allBuffer(orgSize * organismsCount);
    MPI_Datatype orgDType = Organism::getDatatype(this->orgCols, this->orgRows);

    time = MPI_Wtime();

    float alterations = orgRows * orgCols / 10;
    for (size_t epoch = 0; epoch < generations; epoch++)
    {
        // Calculate loss for organis
        Organism &org = *currentOrganism;
        mutate(org, alterations);

        optional<string> debugSave = nullopt;
        if (rank == 0 && epoch % 10 == 0)
            debugSave = string("debug/Debug-") + to_string(epoch) + ".html";

        float stress = evaluator.GetPerformance(*org.field, debugSave);
        if (stress > maxStress) // Mechanical structure broke, organism died
            org.loss = INFINITY;
        else
            org.loss = org.countPlanes();

        //Node serialize the organism
        org.writeIntoBuffer(singleBuffer.data());

        //Gather all the organisms
        MPI_Gather(singleBuffer.data(), 1, orgDType, allBuffer.data(), organismsCount, orgDType, 0, MPI_COMM_WORLD);

        if (rank == 0)
        {
            //deserialize and populate vector
            for (size_t i = 0; i < organismsCount; i++)
                (*currentGeneration)[i].readFromBuffer(singleBuffer.data() + i * orgSize);

            // Sort according to loss
            sort(currentGeneration->begin(), currentGeneration->end(), [](Organism &a, Organism &b) {return a.loss < b.loss; });
            cout << epoch << " ALT: " << alterations << " Planes: " << (*currentGeneration)[0].loss << endl;

            //serialize best one
            (*currentGeneration)[0].writeIntoBuffer(singleBuffer.data()); 
        }

        MPI_Bcast(singleBuffer.data(), 1, orgDType, 0, MPI_COMM_WORLD);
        currentOrganism->readFromBuffer(singleBuffer.data());

        // Restart from the best elected organism
        mutate(*currentOrganism, alterations);
        alterations *= alterationDecay;
    }

    time = MPI_Wtime() - time;

    double maxtime, mintime, avgtime;

    MPI_Reduce(&time, &maxtime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&time, &mintime, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&time, &avgtime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        avgtime /= organismsCount;
        cout << std::fixed << std::setprecision(2) << "Mintime: " << mintime << " Maxtime: " << maxtime << " Avgtime: " << avgtime << endl;
    }
}
