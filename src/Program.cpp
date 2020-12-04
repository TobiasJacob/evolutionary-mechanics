#include <iostream>
#include <iomanip>
#include <string>
#include <omp.h>
#include "Field.hpp"
#include "PerformanceEvaluator.hpp"

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
    float perf;
    perf = evaluator.GetPerformance(field, "debug.html");

    cout << perf << endl;
}
