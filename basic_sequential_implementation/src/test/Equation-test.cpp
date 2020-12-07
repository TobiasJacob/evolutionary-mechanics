#include "catch/catch.hpp"

#include "../Equation.hpp"

TEST_CASE("Solve1", "[Equation]")
{
    Equation equation(2);
    equation.K.Value(0, 0) = 2;
    equation.K.Value(0, 1) = .1;
    equation.K.Value(1, 0) = .1;
    equation.K.Value(1, 1) = 2;
    equation.f[0] = 4.1;
    equation.f[1] = 2.2;
    equation.SolveIterative();
    INFO("Required " << equation.GetSteps() << " steps");
    REQUIRE(equation.GetSolution()[0] == Approx(2).margin(1e-10));
    REQUIRE(equation.GetSolution()[1] == Approx(1).margin(1e-10));
}