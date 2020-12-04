#include <iostream>
#include <iomanip>
#include "Field.hpp"
#include "PerformanceEvaluator.hpp"
#include <string>
#include "EvolutionaryOptimizator.hpp"

using namespace std;

typedef struct{
    Field* field;
    int rows, cols;
}organism;

unsigned int testOrg(organism orgToTest)
{
	return *(unsigned int *)orgToTest.field;
}

void progressDisplayer(unsigned int currentFitness)
{
	printf("fitness: %u\n", currentFitness);
}

int main(int argc, char **argv)
{
    if (argc != 2) 
    {
        cout << "Usage: " << argv[0] << " <N>" << endl;
        exit(1);
    }
    int N = stoi(argv[1]);

    // Support and Forces should remain unchanged during the remaining part of the program
    Support support = {
        .RowSupports = {{ // Define vertical-blocking support on bottom, second from left corner
            .row = 0,
            .col = N
        }},
        .ColSupports = {{ // Define vertical-blocking support on bottom left corner
            .row = 0,
            .col = 0
        }}
    };
    for (int i = 0; i < N; i++) {
        support.RowSupports.push_back({
            .row = 0,
            .col = i
        });
    }

    vector<Force> forces(N + 1);
    for (int i = 0; i < forces.size(); i++)
    {
        forces[i] = {
            .attackCorner = {
                .row = N,
                .col = i
            },
            .forceRow = .1,
            .forceCol = 0
        };
    }
    

    // Field defines the structural layout
    Field field(N, N);
    for (int i = 0; i < N; i++)
        for (int i2 = 0; i2 < N; i2++)
            if (i < N / 3 || i > 2 * N / 3 || (i2 > N / 3 && i2 < 2 * N / 3))
                field.Plane(i, i2) = true;
    

    PerformanceEvaluator evaluator(N, N, support, forces); 
    float perf = evaluator.GetPerformance(field, "debug.html");

    cout << perf << endl;
    EvolutionaryOptimizator::organism *orgs = new EvolutionaryOptimizator::organism[2];
    EvolutionaryOptimizator::organism org1, org2;
    org1.field = &field;
    org2.field = &field;
    
    org1.rows = N;
    org1.cols = N;
    org2.rows = N;
    org2.cols = N;
    
    orgs[0] = org1;
    orgs[1] = org2;

    EvolutionaryOptimizator ea(orgs, support, forces, 2, 0, N, N);
    
    EvolutionaryOptimizator::organism finishedOrg = ea.evolve();
    cout <<"Done"<<endl;
}
