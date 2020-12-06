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
    int rank, size, namelen;
    double time,maxtime,mintime,avgtime;
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    vector<byte> buffer(Organism::getSize(this->orgCols, this->orgRows));
    size_t bufferSize = Organism::getSize(this->orgCols, this->orgRows);

    MPI_Barrier(MPI_COMM_WORLD);
    time = MPI_Wtime();

    float alterations = orgRows * orgCols / 10;
    for (size_t epoch = 0; epoch < generations; epoch++)
    {
        // Calculate loss for organis
        Organism &org = (*currentOrganism);

        float stress;
        stress = evaluator.GetPerformance(*org.field, debugSave);
        if (stress > maxStress) // Mechanical structure broke, organism died
            org.loss = INFINITY;
        else
            org.loss = org.countPlanes();

        if(rank == 0){
            unique_ptr<<vector<organism>> producedGeneration;
            vector<byte> rbuffer(Organism::getSize(this->orgCols, this->orgRows) * size);
        }

        //Node serialize the organism
        org.writeIntoBuffer(buffer.data());

        //Gather all the organisms
        MPI_Gather(buffer.data(), (int)bufferSize, MPI_BYTE, rbuffer.data(), ((int)buffersize * size), MPI_BYTE, 0, MPI_COMM_WORLD);

        if(rank == 0){
         
            //deserialize and populate vector

            // Sort according to loss
            sort(producedGeneration->begin(), producedGeneration->end(), [](Organism &a, Organism &b) {return a.loss   < b.loss; });
            cout << epoch << " ALT:" << alterations << " Planes: " << (*currentOrganism)[0].loss << endl;

            //serialize best one
            producedGeneration.front();
            org.writeIntoBuffer(buffer.data()); 
        }


        MPI_BCast(buffer.data(), 1, MPI_BYTE, 0, MPI_COMM_WORLD);

        //Deserialize the organism
        
        nextOrganism.readFromBuffer(buffer.data());

        // Restart from the best elected organism
        mutate((*nextOrganism), alterations);
        currentOrganism = nextOrganism;
        alterations *= alterationDecay;
    }

    time = MPI_Wtime() - time;

    MPI_Reduce(&mytime, &maxtime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&mytime, &mintime, 1, MPI_DOUBLE, MPI_MIN, 0,MPI_COMM_WORLD);
    MPI_Reduce(&mytime, &avgtime, 1, MPI_DOUBLE, MPI_SUM, 0,MPI_COMM_WORLD);

    if (my_rank == 0) {
        avgtime /= size;
        cout << std::fixed << std:setprecision(2) << "Min: " << mintime << " Max: << " << maxtime "  Avg: " << avgtime << endl;
     }

     MPI_FINALIZE();
}
