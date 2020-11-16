#include "PerformanceEvaluator.hpp"
#include "Equation.hpp"

const static float K[6][6] = {
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

    // Add three constrains for global position and rotation
    Equation equation(2 * field.GetCounter(), 2 * field.GetCounter() + 3);

    for (int r = 0; r < field.Rows; r++)
        for (int c = 0; c < field.Cols; c++)
            if (field.Plane(r, c))
            {
                // Lower left triangle
                const int targetIndicesLower[6] = {
                    2 * field.CornerIndex(r, c) - 2,
                    2 * field.CornerIndex(r, c) - 1,
                    2 * field.CornerIndex(r + 1, c) - 2,
                    2 * field.CornerIndex(r + 1, c) - 1,
                    2 * field.CornerIndex(r, c + 1) - 2,
                    2 * field.CornerIndex(r, c + 1) - 1
                };

                for (int i = 0; i < 6; i++)
                    for (int j = 0; j < 6; j++)
                        equation.K.Value(targetIndicesLower[i], targetIndicesLower[j]) += K[i][j];

                // Upper right triangle
                const int targetIndicesUpper[6] = {
                    2 * field.CornerIndex(r + 1, c + 1) - 2,
                    2 * field.CornerIndex(r + 1, c + 1) - 1,
                    2 * field.CornerIndex(r, c + 1) - 2,
                    2 * field.CornerIndex(r, c + 1) - 1,
                    2 * field.CornerIndex(r + 1, c) - 2,
                    2 * field.CornerIndex(r + 1, c) - 1
                };

                for (int i = 0; i < 6; i++)
                    for (int j = 0; j < 6; j++)
                        equation.K.Value(targetIndicesUpper[i], targetIndicesUpper[j]) += K[i][j];
            }
    
    equation.Print();
    
    return 0;
}

