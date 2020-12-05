#include "catch/catch.hpp"
#include <omp.h>

#include "../PerformanceEvaluator.hpp"

void testWithSize(ostream &out, size_t N, size_t C)
{
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

    // Field defines the structural layout
    Field field(N, N);
    for (size_t i = 0; i < N; i++)
        for (size_t i2 = 0; i2 < N; i2++)
            if (i < N / 3 || i > 2 * N / 3 || (i2 > N / 3 && i2 < 2 * N / 3))
                field.Plane(i, i2) = true;
    

    PerformanceEvaluator evaluator(N, N, support, forces);
    
    evaluator.GetPerformance(field, nullopt);

    out << N << ", " << C << ", " << evaluator.GetLastSolvingTime() << endl;
}

TEST_CASE("PerformanceEvaluatorSpeedup", "[.][PerformanceEvaluator]")
{
    if (true)
    {
        ofstream output("performance.txt");
        output << "N, C, Time" << endl;
        for (size_t N : {10, 25, 50, 75, 100, 150, 200})
            for (size_t C : {1, 2, 4, 8, 16})
                testWithSize(output, N, C);
    }
    else
    {
        testWithSize(cout, 100, 8);
    }
}