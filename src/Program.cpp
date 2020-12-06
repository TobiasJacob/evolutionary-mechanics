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
    MPI_Init(&argc, &argv);
    srand(0); // Reproducable behaviour

    if (argc != 2) 
    {
        cout << "Usage: " << argv[0] << " <N> " << endl;
        exit(1);
    }
    
    size_t N = (size_t)stoi(argv[1]);

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

    vector<Force> forces(N / 3);
    for (size_t i = 0; i < forces.size(); i++)
    {
        forces[i] = {
            .attackCorner = {
                .row = N,
                .col = i
            },
            .forceRow = .01,
            .forceCol = .1
        };
    }
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int organisms = 100 - 100 % size;
    EvolutionaryOptimizator evolutionary_optimizator(support, forces, organisms, N, N);
    
    evolutionary_optimizator.Evolve(200, 1.f, 0.995f);

    MPI_Finalize();
}

