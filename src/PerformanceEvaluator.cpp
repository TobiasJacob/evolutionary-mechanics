#include "PerformanceEvaluator.hpp"

double PerformanceEvaluator::getPerformance(Field &field)
{
    // Dummy implementation that returns the number of blocks
    double result = 0;
    for (int i = 0; i < field.Rows; i++)
        for (int j = 0; j < field.Cols; j++)
            result += field.field(i, j);
    
    return result;
}

