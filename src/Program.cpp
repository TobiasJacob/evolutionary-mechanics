#include <iostream>
#include <iomanip>
#include "Field.hpp"
#include "PerformanceEvaluator.hpp"
#include <string>

using namespace std;

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
        .SupportRow1 = { // Define vertical-blocking support on bottom left corner
            .row = 0,
            .col = 0
        },
        .SupportRow2 = { // Define vertical-blocking support on bottom, second from left corner
            .row = 0,
            .col = N
        },
        .SupportCol = { // Define horizontal-blocking support  on bottom left corner
            .row = 0,
            .col = 0
        }
    };
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
            if (i < N / 3 || i > 2 * N / 3 || i2 < N / 3)
                field.Plane(i, i2) = true;
    

    PerformanceEvaluator evaluator(N, N, support, forces);
    float perf = evaluator.GetPerformance(field, "debug.html");

    cout << perf << endl;
}
