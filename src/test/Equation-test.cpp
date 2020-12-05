#include "catch/catch.hpp"

#include "../Equation.hpp"

TEST_CASE("Solve1", "[Equation]")
{
    Equation equation(2);
    equation.K.GetOrAllocateValue(0, 0) = 2;
    equation.K.GetOrAllocateValue(0, 1) = .1;
    equation.K.GetOrAllocateValue(1, 0) = .1;
    equation.K.GetOrAllocateValue(1, 1) = 2;
    equation.f[0] = 4.1;
    equation.f[1] = 2.2;
    #pragma omp parallel
    equation.SolveIterative();
    INFO("Required " << equation.GetSteps() << " steps");
    REQUIRE(equation.GetSolution()[0] == Approx(2).margin(1e-10));
    REQUIRE(equation.GetSolution()[1] == Approx(1).margin(1e-10));
}