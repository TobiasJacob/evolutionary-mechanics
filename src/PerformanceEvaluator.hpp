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
    const int rows, cols;
    const Support &supports;
    const vector<Force> &forces;

    int cornerCounter; // The maximum number
    Matrix<int> cornerIndex; // A unique number for each corner (after calculateIndex)

    Equation setupEquation(Field &field);
    float calculateMaxStress(Field &field, const vector<float> &q);
    void refreshCornerIndex(Field &field);
public:
    PerformanceEvaluator(const int rows, const int cols, const Support &supports, const vector<Force> &forces);
    float GetPerformance(Field &field);
};

#endif
