#include <iostream>
#include <iomanip>
#include "Field.hpp"
#include "PerformanceEvaluator.hpp"
#include <string>

using namespace std;

int main(int argc, char **argv)
{
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
    vector<Force> forces = {
        {
            .attackCorner = {
                .row = N,
                .col = N
            },
            .forceRow = 10,
            .forceCol = 0
        }
    };

    // Field defines the structural layout
    Field field(N, N);
    for (int i = 0; i < N; i++)
        for (int i2 = 0; i2 < N; i2++)
            field.Plane(i, i2) = true;
    

    PerformanceEvaluator evaluator;
    float perf = evaluator.GetPerformance(field, support, forces);

    cout << perf << endl;
}
