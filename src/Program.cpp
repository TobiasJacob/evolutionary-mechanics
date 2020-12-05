#include <iostream>
#include <iomanip>
#include <string>
#include <omp.h>
#include "Field.hpp"
#include "PerformanceEvaluator.hpp"
<<<<<<< HEAD
#include <string>
#include "EvolutionaryOptimizator.hpp"
=======
>>>>>>> master

using namespace std;

int main(int argc, char **argv)
{
    if (argc != 3) 
    {
        cout << "Usage: " << argv[0] << " <N> <C>" << endl;
        exit(1);
    }
    size_t N = (size_t)stoi(argv[1]);
    size_t C = (size_t)stoi(argv[2]);

    omp_set_num_threads(C);

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
    for (size_t i = 0; i < N; i++) {
        support.RowSupports.push_back({
            .row = 0,
            .col = i
        });
    }

    vector<Force> forces(N + 1);
    for (size_t i = 0; i < forces.size(); i++)
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
    for (size_t i = 0; i < N; i++)
        for (size_t i2 = 0; i2 < N; i2++)
            if (i < N / 3 || i > 2 * N / 3 || (i2 > N / 3 && i2 < 2 * N / 3))
                field.Plane(i, i2) = true;
    

    PerformanceEvaluator evaluator(N, N, support, forces); 
    double desiredFitness = 0.05;
    int numberOfOrganisms = 4;

    float perf = evaluator.GetPerformance(field, "debug.html");

    cout << "Starting Performance: " << perf << endl;
    cout << "Desired Fitness: " << desiredFitness << endl;
    cout << "Starting number of organisms: " << numberOfOrganisms << endl;
    
    EvolutionaryOptimizator::organism *orgs = new EvolutionaryOptimizator::organism[4];
    EvolutionaryOptimizator::organism org1, org2, org3, org4, org5;

    org1.field = &field;
    org2.field = &field;
    org3.field = &field;
    org4.field = &field;
    
    orgs[0] = org1;
    orgs[1] = org2;
    orgs[2] = org3;
    orgs[3] = org4;

    EvolutionaryOptimizator evolutionary_optimizator(orgs, support, forces, numberOfOrganisms, desiredFitness, N, N);
    
    evolutionary_optimizator.evolve();
    cout <<"Done"<<endl;
}

