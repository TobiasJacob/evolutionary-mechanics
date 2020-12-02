#include "catch/catch.hpp"

#include "../PerformanceEvaluator.hpp"

TEST_CASE("PerformanceEvaluator", "[PerformanceEvaluator]")
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
    
    PerformanceEvaluator evaluator(3, 3, support, forces);
    float performance = evaluator.GetPerformance(field);
    INFO("Perf: " << performance);
    REQUIRE(performance == Approx(1029.35278f).margin(1e-2));
}

TEST_CASE("PerformanceEvaluatorSupportNotAttached", "[PerformanceEvaluator]")
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
                .col = 2
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
    field.Plane(2, 0) = false;
    
    PerformanceEvaluator evaluator(3, 3, support, forces);
    float performance = evaluator.GetPerformance(field);
    INFO("Perf: " << performance);
    REQUIRE(performance == INFINITY);
}
TEST_CASE("PerformanceEvaluatorForceNotAttached", "[PerformanceEvaluator]")
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
                .row = 3,
                .col = 2
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
    
    PerformanceEvaluator evaluator(3, 3, support, forces);
    float performance = evaluator.GetPerformance(field);
    INFO("Perf: " << performance);
    REQUIRE(performance == INFINITY);
}

TEST_CASE("DoubleForceMeansFourTimesPenality", "[PerformanceEvaluator]")
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
    
    PerformanceEvaluator evaluator(3, 3, support, forces);
    float performance1 = evaluator.GetPerformance(field);
    INFO("Perf1: " << performance1);
    forces[0].forceRow = 20;
    float performance2 = evaluator.GetPerformance(field);
    INFO("Perf2: " << performance2);
    REQUIRE(4 * performance1 == Approx(performance2).margin(1e-2));
}

TEST_CASE("ForceZeroMeansPerformanceZero", "[PerformanceEvaluator]")
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
            .forceRow = 0,
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
    
    PerformanceEvaluator evaluator(3, 3, support, forces);
    float performance1 = evaluator.GetPerformance(field);
    INFO("Perf1: " << performance1);
    REQUIRE(performance1 == Approx(0.0f).margin(1e-2));
}