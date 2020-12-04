#include "EvolutionaryOptimizator.hpp"
#include "PerformanceEvaluator.hpp"
#include "Equation.hpp"
#include "Matrix.hpp"
#include "Microtime.hpp"
#include <fstream>
#include <algorithm>
#include <random>
#include <iterator>

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}

struct sorting_element{
    unsigned int fitness;
    int state;
    struct sorting_element *nextElement;
};

bool compareByFitness(const EvolutionaryOptimizator::organism &a, const EvolutionaryOptimizator::organism &b)
{
    return a.fitness < b.fitness;
}

EvolutionaryOptimizator::EvolutionaryOptimizator(EvolutionaryOptimizator::organism *organisms, const Support &supports, const vector<Force> &forces, const int organismsCount, const double desiredFitness, const int orgRows,  const int orgCols) : organisms(organisms), supports(supports), forces(forces), organismsCount(organismsCount), desiredFitness(desiredFitness), orgRows(orgRows), orgCols(orgCols){
}


void EvolutionaryOptimizator::copyOrganism(EvolutionaryOptimizator::organism org1, EvolutionaryOptimizator::organism org2, int rows, int cols, int startingRow, int startingCol)
{   
    for(int r = startingRow; r < rows; r++){

        for(int c = startingCol; c < cols; c++){
            org2.field->Plane(r,c) = org1.field->Plane(r,c);
        }
    }
    
    return;
}


void EvolutionaryOptimizator::simpleCrossingOver(EvolutionaryOptimizator::organism org1, EvolutionaryOptimizator::organism org2, EvolutionaryOptimizator::organism dest, int rows, int cols)
{
    for(int r = 0; r < rows; r++){
        
        for(int c = 0; c < cols; c++){
            
            if(org1.field->Plane(r,c) && org2.field->Plane(r,c)){
                dest.field->Plane(r,c) = true; 
            }

            else if((org1.field->Plane(r,c) || org2.field->Plane(r,c)) && rand()%2 == 0){
                dest.field->Plane(r,c) = true;      
            }

            else{
                dest.field->Plane(r,c) = false;
            }
        }
    }

    return;
}

EvolutionaryOptimizator::organism EvolutionaryOptimizator::evolve()
{   
    float rest = this->organismsCount % 2;

    if(rest){
        throw "Odd number of organisms. Check.";
    }
    
    //Seed random number generator
    srand(time(NULL));

    EvolutionaryOptimizator::organism orgToReturn;
    orgToReturn.rows = 0;
    orgToReturn.cols = 0;

    unsigned long long totalFitness = 0;

    //current generation organisms buffer
    EvolutionaryOptimizator::organism *currentGeneration = new EvolutionaryOptimizator::organism[this->organismsCount];

    memcpy(currentGeneration, this->organisms, this->organismsCount * sizeof(EvolutionaryOptimizator::organism));

    //next generation organisms buffer

    EvolutionaryOptimizator::organism *newGeneration = new EvolutionaryOptimizator::organism[MAX_NUMBER_OF_REPRODUCTIONS];

    int totalChildren = this->organismsCount;

    //Counters
    int i;

    
    PerformanceEvaluator evaluator(this->orgRows, this->orgCols, this->supports, this->forces);
    bool satisfied = false;

    while(!satisfied){
        double orgFitness = 0;

        //Loops thru organisms and tests fitness + sorting

        for(int i=0; i < totalChildren; i++){

            orgFitness = evaluator.GetPerformance(*this->organisms[i].field, "debug.html");

            cout << "Old Fitness: " << orgFitness << endl;  

            //if org found with satysfactory fitness
            if(orgFitness >= this->desiredFitness){
                orgToReturn = this->organisms[i];
                //check we need to save the new field s
	            orgToReturn.field = new Field(this->orgRows, this->orgCols);
                copyOrganism(this->organisms[i], orgToReturn, this->orgRows, this->orgCols, 0, 0);    
                satisfied = true;
            }
            
            this->organisms[i].fitness = orgFitness;
            this->organisms[i].state = 1;
            
            elements.push_back(organisms[i]);
            totalFitness += orgFitness;
            
        }

        //Reproduction
        
        //Next generation array last index
        int newGenerationIndex = 0;
        
        //Decides number of children to have

        int children = 0;
        int randomNumber = 0;
        totalChildren = 0;
        int reproductions = 0;

        while(reproductions != 5){
          
            EvolutionaryOptimizator::organism to_replicate = *select_randomly(elements.begin(), elements.end());
            EvolutionaryOptimizator::organism mate = *select_randomly(elements.begin(), elements.end());
 
            //Calculates number of children based on both parents fitness
            //children = (int) (this->organismsCount * (double) (to_replicate.fitness + mate.fitness)/totalFitness);
            children = 5;
            totalChildren += children;
            for(int j = 0; j<children; j++){
                newGeneration[newGenerationIndex] = reproduce(to_replicate, mate);
                
                //cout << *newGeneration[newGenerationIndex].field << endl; 
                newGenerationIndex++;
        
   //             orgFitness = evaluator.GetPerformance(*newGeneration[i].field, "debug.html");
     //           cout << "New Fitness: " << orgFitness << endl;  
            }

            reproductions ++; 
        }
        
        
        //This is just some clthis->-up. It makes sure there are an even number of organisms in the population, so this->h one has a mate. It also frees allocated DNA and buffer memory.
       
        if((totalChildren % 2)){

            EvolutionaryOptimizator::organism dummyOrg;
            dummyOrg.rows = 0;
            dummyOrg.cols = 0;
            dummyOrg.field = new Field(0,0);

            newGeneration[newGenerationIndex] = dummyOrg;
            totalChildren ++;
        }

        EvolutionaryOptimizator::organism *buffer = newGeneration;
        newGeneration = (EvolutionaryOptimizator::organism *)memset(&currentGeneration, 0, sizeof(currentGeneration));
        currentGeneration = buffer;
        totalFitness = 0;
   }

    //delete[] currentGeneration;
    //delete[] newGeneration;
    cout << "Winner Structure\n" << *orgToReturn.field << endl;
    double fit = evaluator.GetPerformance(*orgToReturn.field, "debug.html");
    cout << "Performance\n" << fit << endl;
    return orgToReturn;
}

EvolutionaryOptimizator::organism EvolutionaryOptimizator::reproduce(EvolutionaryOptimizator::organism org1, EvolutionaryOptimizator::organism org2)
{
    EvolutionaryOptimizator::organism child;
    //Decides at which index to start crossing with other parent
    int crossingCols;
    int crossingRows;
    //Number of mutations (in base pairs) in child
    int mutationCount;
    //Base pair to mutate
    int mutatedBasePair;
    //Mutation base pair buffer
    unsigned char basePairBuffer;
    //Counter
    int i;

    child.field = new Field(org1.field->Rows, org1.field->Cols);

    //Crossing over
/*

    //copyOrganism(child, org1, org1.field->Rows, org1.field->Cols, 0, 0);
   crossingCols = rand() % (this->orgCols + 1);
    crossingRows = rand() % (this->orgRows + 1);

    int rowsOffset = this->orgRows - crossingRows >= (int)round((double)this->orgRows / 2.0) ?
	                    (int)round((double)this->orgRows / 2.0) : this->orgRows - crossingRows;
    
    int colsOffset = this->orgCols - crossingCols >= (int)round((double)this->orgCols / 2.0) ?
	                    (int)round((double)this->orgCols / 2.0) : this->orgCols - crossingCols;


    //Copying from somewhere in middle of dna to end, or half of the dna length
    copyOrganism(child, org2, rowsOffset, colsOffset, crossingRows, crossingCols);

    //If half of the dna length isn't copied alrthis->y, the remaining amount is copied from the beginning (wraps around)
    rowsOffset =  this->orgRows - crossingRows >= (int)round((double)this->orgRows / 2.0) ?
        0 : ((int)round((double)this->orgRows/ 2.0) - (this->orgRows - crossingRows)); 
    colsOffset =  this->orgCols - crossingCols >= (int)round((double)this->orgCols / 2.0) ?
        0 : ((int)round((double)this->orgCols/ 2.0) - (this->orgCols - crossingCols)); 

    //copyOrganism(child, org2, rowsOffset, colsOffset, crossingRows, crossingCols);
*/

    simpleCrossingOver(org1,org2, child, org1.field->Rows, org1.field->Cols);
    
    //Mutating
    mutationCount = rand() % (2 * (int)round(MUTATIONS_PER_BASE_PAIR_CONSTANT * (double)this->orgRows * (double)this->orgCols * 8.0));

    for (i = 0; i < mutationCount; i++) {

        //Chooses a random Col and a random row to execute the mutation
        int mutationCol = (rand() % (this->orgCols));
        int mutationRow = (rand() % (this->orgRows));

        if(mutationRow != 0 && mutationRow!= this->orgRows -1  && mutationCol != 0 && mutationCol != this->orgCols - 1){
            //If there are not elements mutate only if there is an element as neighbour 
            if(!child.field->Plane(mutationRow, mutationCol)) {
               if(child.field->Plane(mutationRow, mutationCol - 1) || 
                       child.field->Plane(mutationRow, mutationCol + 1) ||
                       child.field->Plane(mutationRow - 1, mutationCol) ||
                       child.field->Plane(mutationRow + 1, mutationCol)){

                   child.field->Plane(mutationRow, mutationCol) = true;
               }
            }
        
           //If there is an element mutate only if its not in the middle of two neighbours
           else{

               if(!(child.field->Plane(mutationRow, mutationCol -1) && (child.field->Plane(mutationRow, mutationCol + 1))) ||
                           !((child.field->Plane(mutationRow -1, mutationCol) && (child.field->Plane(mutationRow + 1, mutationCol))))){
                   
                   child.field->Plane(mutationRow, mutationCol) = false;
               }
           }
        }

    }
    return child;
}

