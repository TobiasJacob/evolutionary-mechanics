#include <iostream>
#include <iomanip>
#include <string>
#include "Field.hpp"
#include "PerformanceEvaluator.hpp"
#include "EvolutionaryOptimizator.hpp"

using namespace std;

int main(int argc, char **argv)
{
    if (argc != 5) 
    {
        cout << "Usage: " << argv[0] << " <N> <Organisms> <Epochs> <Decay>, recommend 20 100 1000 0.995f" << endl;
        exit(1);
    }
    
    size_t N = (size_t)stoi(argv[1]);
    size_t organisms = (size_t)stoi(argv[2]);
    size_t epochs = (size_t)stoi(argv[3]);
    float decay = atof(argv[4]);

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
            .forceCol = 1.f / (float)N
        };
    }

    EvolutionaryOptimizator evolutionary_optimizator(support, forces, organisms, N, N);
    
    evolutionary_optimizator.Evolve(epochs, 1.f, decay);
}
