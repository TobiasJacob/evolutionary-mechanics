#include "EvolutionaryOptimizator.hpp"

struct sorting_element{
    unsigned int fitness;
    int state;
    struct sorting_element *nextElement;
};

EvolutionaryOptimizator::organism EvolutionaryOptimizator::evolve(EvolutionaryOptimizator::evolutionary_algorithm ea)
{
    if(ea.organismsCount % 2){
        std::cout << "Error in evolve: Cannot have an odd number of organisms" << std::endl;
        EvolutionaryOptimizator::organism nullOrg;

        //We should return a null invalid Field
        nullOrg.field = new Field(0,0);
        nullOrg.rows = 0;
        nullOrg.cols = 0;
        
        return nullOrg;
    }

    //Seed random number generator
    srand(time(NULL));

    EvolutionaryOptimizator::organism orgToReturn;
    orgToReturn.rows = 0;
    orgToReturn.cols = 0;

    unsigned long long totalFitness = 0;

    //current generation organisms buffer
    EvolutionaryOptimizator::organism *currentGeneration = new EvolutionaryOptimizator::organism[ea.organismsCount];

    memcpy(currentGeneration, ea.organisms, ea.organismsCount * sizeof(EvolutionaryOptimizator::organism));

    //next generation organisms buffer

    EvolutionaryOptimizator::organism *newGeneration = new EvolutionaryOptimizator::organism[ea.organismsCount];

    int totalChildren = ea.organismsCount;

    //Counters
    int i,j;

    //Used to sort the organisms from most to least fit
    //First element is for placholding(?)

    //Check this
    struct sorting_element *elements = (struct sorting_element *) malloc(sizeof(struct sorting_element) * (ea.organismsCount + 1));
    
    elements[0].fitness = 0.0;
    elements[0].nextElement = NULL;
    elements[0].state = 0;
    
    while(1){

        unsigned int orgFitness = 0;

        //Loops thru organisms and tests fitness + sorting

        for(i=0; i < totalChildren; i++){
            PerformanceEvaluator evaluator;

            orgFitness = evaluator.GetPerformance(*ea.organisms[i].field, this->supports, this->forces);
            orgFitness = ea.fitnessTest(ea.organisms[i]);

            //if org found with satysfactory fitness
            if(orgFitness >= ea.desiredFitness){
                orgToReturn = ea.organisms[i];
                //check we need to save the new field s
	            orgToReturn.field = new Field(ea.orgRows, ea.orgCols);
                copyOrganism(ea.organisms[i], orgToReturn, ea.orgRows, ea.orgCols, 0, 0);    
            }

            elements[i + 1].fitness = orgFitness;
            elements[i + 1].nextElement = NULL;
            elements[i + 1].state = 1;
            totalFitness += orgFitness;
            
            //Sorts orgs using relative values
            if (!i) {
                elements[0].nextElement = &elements[1];
                continue;
            }

            //Used to iterate thru array of organism fitnesses
            struct sorting_element *next = &elements[0];
            
            for(j = 0; j < i+1; j++){

                if(next-> nextElement == NULL){

                    next->nextElement = &elements[i + 1];
                    break;
                }

                if(next->nextElement->fitness <= orgFitness){

                    elements[i + 1].nextElement = next->nextElement;
                    next->nextElement = &elements[i + 1];
                    break;
                }
                next = next->nextElement;
            }
        }

        ea.progress(elements[0].nextElement->fitness);

        if(orgToReturn.field == NULL){
            break;
        }

        //Reproduction
        
        //Mate finder pointers
        struct sorting_element *nextMate;
        struct sorting_element *previousMate;

        //Mate pointer
        EvolutionaryOptimizator::organism *mate;
        
        //Organism buffer in reproduction
        struct sorting_element *elementToReplicate;

        //Next generation array last index
        int newGenerationIndex = 0;
        //Decides number of children to have

        int children = 0;
        int randomNumber = 0;
        totalChildren = 0;

        elementToReplicate = elements[0].nextElement;

        while(elementToReplicate != NULL){
            EvolutionaryOptimizator::organism *orgToReplicate = 
                &ea.organisms[(int) (elementToReplicate - elements) -1];
            previousMate = NULL;

            if(!elementToReplicate->state){
                elementToReplicate = elementToReplicate->nextElement;
                continue;
            }

            //Moves down the hierarchy a maximum of MATE_VARIANCE_CONSTANT percent
            for(j = 0; j < randomNumber; j++){

                if(nextMate->state){
                    previousMate = nextMate;
                }

                nextMate = nextMate->nextElement;

                if(nextMate == NULL){
                    while(!nextMate->state){

                        nextMate = nextMate->nextElement;
                    }
                }

                else{

                    nextMate = previousMate;
                }
            }

            mate = &ea.organisms[(int) (nextMate - elements) - 1];
            nextMate->state = 0;

            //Calculates number of children based on both parents fitness
            children = (int) (ea.organismsCount * (double) 
                    (elementToReplicate->fitness + nextMate->fitness)/totalFitness);
            
            totalChildren += children;
            for(j = 0; j<children; j++){

                newGeneration[newGenerationIndex] = reproduce(*orgToReplicate, *mate, ea);
                newGenerationIndex++;
            }

            free(orgToReplicate->field);
            free(mate->field);
            elementToReplicate = elementToReplicate->nextElement;
        }
        
        //This is just some clean-up. It makes sure there are an even number of organisms in the population, so each one has a mate. It also frees allocated DNA and buffer memory.
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
        memset(elements, 0, sizeof(currentGeneration));
        totalFitness = 0;
    }

    struct sorting_element *elementFreeingPointer = elements[0].nextElement;
    struct sorting_element *elementFreeingBuffer;

    while(elementFreeingPointer != NULL){

        elementFreeingBuffer = elementFreeingPointer;
        free(ea.organisms[(int) (elementFreeingPointer - elements) - 1].field);
        elementFreeingPointer = elementFreeingBuffer->nextElement;
    }

    free(currentGeneration);
    free(newGeneration);
    free(elements);

    return orgToReturn;
}

void copyOrganism(EvolutionaryOptimizator::organism org1, EvolutionaryOptimizator::organism org2, int rows, int cols, int startingRow, int startingCol)
{   
    for(int r = startingRow; r < rows; r++){

        for(int c = startingCol; c < cols; c++){

            org2.field->Plane(r,c) = org1.field->Plane(r,c);
        }
    }
    
    return;
}

EvolutionaryOptimizator::organism reproduce(EvolutionaryOptimizator::organism org1, EvolutionaryOptimizator::organism org2, EvolutionaryOptimizator::evolutionary_algorithm alg)
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

    //Child starts with first parent's dna then crosses with the other
    child.field = new Field(org1.field->Rows, org1.field->Cols);
    copyOrganism(child, org1, org1.field->Rows, org1.field->Cols, 0, 0);

    //Crossing over
    crossingCols = rand() % (alg.orgCols + 1);
    crossingRows = rand() % (alg.orgRows + 1);

    int rowsOffset = alg.orgRows - crossingRows >= (int)round((double)alg.orgRows / 2.0) ?
	                    (int)round((double)alg.orgRows / 2.0) : alg.orgRows - crossingRows;
    
    int colsOffset = alg.orgCols - crossingCols >= (int)round((double)alg.orgCols / 2.0) ?
	                    (int)round((double)alg.orgCols / 2.0) : alg.orgCols - crossingCols;


    //Copying from somewhere in middle of dna to end, or half of the dna length
    copyOrganism(child, org2, rowsOffset, colsOffset, crossingRows, crossingCols);

    //If half of the dna length isn't copied already, the remaining amount is copied from the beginning (wraps around)
    rowsOffset =  alg.orgRows - crossingRows >= (int)round((double)alg.orgRows / 2.0) ?
        0 : ((int)round((double)alg.orgRows/ 2.0) - (alg.orgRows - crossingRows)); 
    colsOffset =  alg.orgCols - crossingCols >= (int)round((double)alg.orgCols / 2.0) ?
        0 : ((int)round((double)alg.orgCols/ 2.0) - (alg.orgCols - crossingCols)); 

    copyOrganism(child, org2, rowsOffset, colsOffset, crossingRows, crossingCols);

    //Mutating

    mutationCount = rand() % (2 * (int)round(MUTATIONS_PER_BASE_PAIR_CONSTANT * (double)alg.orgRows * (double)alg.orgCols * 8.0));

    for (i = 0; i < mutationCount; i++) {

        //Chooses a random Col and a random row to execute the mutation
        int mutationCol = (rand() % (alg.orgCols));
        int mutationRow = (rand() % (alg.orgRows));

        if(mutationRow != (0 || alg.orgRows) && mutationCol != (0 || alg.orgCols) ){
        
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

               if(!(child.field->Plane(mutationRow, mutationCol -1) && (child.field->Plane(mutationRow, mutationCol + 1))) &&
                           !((child.field->Plane(mutationRow -1, mutationCol) || (child.field->Plane(mutationRow + 1, mutationCol))))){
                   
                   child.field->Plane(mutationRow, mutationCol) = false;
               }
           }
        }

    }
    return child;
}

