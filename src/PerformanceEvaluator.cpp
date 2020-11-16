#include "PerformanceEvaluator.hpp"

const static double K[6][6] = {
    { 300,  180, -240,  -60,  -60, -120},
    { 180,  300, -120,  -60,  -60, -240},
    {-240, -120,  240,    0,    0,  120},
    { -60,  -60,    0,   60,   60,    0},
    { -60,  -60,    0,   60,   60,    0},
    {-120, -240,  120,    0,    0,  240}
};

double PerformanceEvaluator::GetPerformance(Field &field)
{
    field.CalculateIndex();
    // Dummy implementation that returns the number of blocks
    double result = 0;
    for (int i = 0; i < field.Rows; i++)
        for (int j = 0; j < field.Cols; j++)
            result += field.Plane(i, j);
    
    return result;
}

