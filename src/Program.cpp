#include <iostream>
#include <iomanip>
#include <string>
#include <omp.h>
#include "Field.hpp"
#include "PerformanceEvaluator.hpp"
#include "EvolutionaryOptimizator.hpp"

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
    srand(0); // Reproducable behaviour

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

    EvolutionaryOptimizator evolutionary_optimizator(support, forces, 100, N, N);
    
    evolutionary_optimizator.Evolve(100, 0.1);
}

