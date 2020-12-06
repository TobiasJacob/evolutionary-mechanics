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
    , globalGeneration(new vector<Organism>(organismsCount, Organism(orgRows, orgCols)))
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
    int rank, size;
    double time;
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    size_t organismsCount = globalGeneration->size();
    const size_t organismsPerNode = organismsCount / size;
    const size_t bestN = organismsCount / 10;
    vector<Organism> localGeneration(organismsPerNode, Organism(orgRows, orgCols));
    
    size_t orgSize = Organism::getSize(this->orgCols, this->orgRows);
    vector<byte> localBuffer(orgSize * organismsPerNode);
    vector<byte> bestBuffer(orgSize * bestN);
    vector<byte> allBuffer(orgSize * organismsCount);
    MPI_Datatype orgDType = Organism::getDatatype(this->orgCols, this->orgRows);

    time = MPI_Wtime();
    float alterations = orgRows * orgCols / 10;
    for (size_t epoch = 0; epoch < generations; epoch++)
    {
        // Calculate loss for organis
        for (size_t i = 0; i < organismsPerNode; i++) 
        {
            mutate(localGeneration[i], alterations);

            optional<string> debugSave = nullopt;
            if (rank == 0 && epoch % 25 == 0 && i == 0)
                debugSave = string("debug/Debug-") + to_string(epoch) + ".html";

            float stress = evaluator.GetPerformance(*localGeneration[i].field, debugSave);
            if (stress > maxStress) // Mechanical structure broke, organism died
                localGeneration[i].loss = INFINITY;
            else
                localGeneration[i].loss = localGeneration[i].countPlanes();

            // Node serialize the organism
            localGeneration[i].writeIntoBuffer(localBuffer.data() + i * orgSize);
        }

        //Gather all the organisms
        MPI_Gather(localBuffer.data(), organismsPerNode, orgDType, allBuffer.data(), organismsPerNode, orgDType, 0, MPI_COMM_WORLD);

        if (rank == 0)
        {
            //deserialize and populate vector
            for (size_t i = 0; i < organismsCount; i++)
                (*globalGeneration)[i].readFromBuffer(allBuffer.data() + i * orgSize);

            // Sort according to loss
            sort(globalGeneration->begin(), globalGeneration->end(), [](Organism &a, Organism &b) {return a.loss < b.loss; });
            if (epoch % 10 == 0)
                cout << epoch << " ALT: " << alterations << " Planes: " << (*globalGeneration)[0].loss << endl;

            // serialize bestN
            for (size_t i = 0; i < bestN; i++)
            {
                (*globalGeneration)[i].writeIntoBuffer(bestBuffer.data() + orgSize * i); 
            }
        }

        MPI_Bcast(bestBuffer.data(), bestN, orgDType, 0, MPI_COMM_WORLD);
        // Calculate loss for organis
        for (size_t i = 0; i < organismsPerNode; i++)
        {
            size_t globalIndex = rank * organismsPerNode + i;
            localGeneration[i].readFromBuffer(bestBuffer.data() + orgSize * (globalIndex % bestN));
        }

        // Restart from the best elected organism
        alterations = (alterations - 1) * alterationDecay + 1;
    }

    time = MPI_Wtime() - time;

    double maxtime;

    MPI_Reduce(&time, &maxtime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << std::fixed << std::setprecision(3) << "Time: " << maxtime << ", Generations: " << generations << ", Epochs per Second: " << generations / maxtime << endl;
        cout << "Used " << size << " node(s) and " << omp_get_max_threads() << " core(s) per node " << endl;
        cout << "Processsed " << this->orgCols << "x" << this->orgRows << " grid and " << organismsCount << " organisms per epoch, resulting in " << organismsPerNode << " organisms per node" << endl;
        cout << "Processing speed: " << generations * organismsCount / maxtime << " organisms per second" << endl;
        cout << "final performance is " << (*globalGeneration)[0].loss << endl;
    }
}
