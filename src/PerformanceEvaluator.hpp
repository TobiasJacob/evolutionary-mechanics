#ifndef PERFORMANCEEVALUATOR
#define PERFORMANCEEVALUATOR

#include <vector>
#include "Field.hpp"

struct Point
{
    int row;
    int col;
};

struct Support
{
    Point SupportRow1; // Sets the row displacement at cornerPoint to zero
    Point SupportRow2; // Sets the row displacement at cornerPoint to zero
    Point SupportCol; // Sets the col displacement at cornerPoint to zero
};

struct Force
{
    Point attackCorner;
    float forceRow;
    float forceCol;
};

class PerformanceEvaluator
{
private:
    Equation SetupEquation(Field &field, Support &supports, vector<Force> forces);
public:
    double GetPerformance(Field &field, Support &supports, vector<Force> forces);
};

#endif