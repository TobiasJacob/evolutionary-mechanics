#include <iostream>
#include <iomanip>
#include "Field.hpp"
#include "PerformanceEvaluator.hpp"

using namespace std;

int main()
{
    // Support and Forces should remain unchanged during the remaining part of the program
    Support support = {
        .SupportRow1 = { // Define vertical-blocking support on bottom left corner
            .row = 3,
            .col = 0
        },
        .SupportRow2 = { // Define vertical-blocking support on bottom, second from left corner
            .row = 3,
            .col = 1
        },
        .SupportCol = { // Define horizontal-blocking support  on bottom left corner
            .row = 3,
            .col = 0
        }
    };
    vector<Force> forces = {
        {
            .attackCorner = {
                .row = 1,
                .col = 3
            },
            .forceRow = 10,
            .forceCol = 0
        }
    };

    // Field defines the structural layout
    Field field(3, 3);
    field.Plane(0, 0) = true;
    field.Plane(1, 0) = true;
    field.Plane(1, 1) = true;
    field.Plane(1, 2) = true;
    field.Plane(2, 0) = true;
    
    PerformanceEvaluator evaluator;
    cout << evaluator.GetPerformance(field, support, forces) << endl;
}

