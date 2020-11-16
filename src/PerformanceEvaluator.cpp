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

double PerformanceEvaluator::GetPerformance(Field &field, Support &supports, vector<Force> forces)
{
    // Refresh corner numbering
    field.CalculateIndex();

    // Add three constrains for global position and rotation
    const int conditions = 2 * field.GetCounter();
    Equation equation(conditions);

    // Set up equations for the supports. If one support is not attached to the field, field failed.
    // Supports are no unkown values anymore.
    // Support values must be different from each other.
    int supportColIndex = 2 * field.CornerIndex(supports.SupportCol.row, supports.SupportCol.col) - 1;
    int supportRow1Index = 2 * field.CornerIndex(supports.SupportRow1.row, supports.SupportRow1.col) - 2;
    int supportRow2Index = 2 * field.CornerIndex(supports.SupportRow2.row, supports.SupportRow2.col) - 2;
    if (supportColIndex < 0 || supportRow1Index < 0 || supportColIndex < 0)
        exit(1);
    if (supportRow1Index == supportRow2Index)
        exit(1);

    // Add forces to equation. If force is not attached to model, model failed.
    // Forces on supports are automatically removed later
    for (Force &f: forces)
    {
        int cornerIndex = field.CornerIndex(f.attackCorner.row, f.attackCorner.col);
        if (!cornerIndex) return INFINITY;
        equation.f[2 * cornerIndex - 2] += f.forceRow;
        equation.f[2 * cornerIndex - 1] += f.forceCol;
    }

    // Setup stiffness matrix
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
    
    // The values of the supports are certain, and can therefore be removed.
    Equation reducedEquation(conditions - 3);

    int rSource = 0;
    for (int r = 0; r < conditions - 3; r++)
    {
        if (rSource == supportColIndex || rSource == supportRow1Index || rSource == supportRow2Index)
            rSource++;
        reducedEquation.f[r] = equation.f[r];
        int cSource = 0;
        for (int c = 0; c < conditions - 3; c++)
        {
            if (cSource == supportColIndex || cSource == supportRow1Index || cSource == supportRow2Index)
                cSource++;
            reducedEquation.K.Value(r, c) = equation.K.Value(r, c);
            cSource++;
        }
        rSource++;
    }

    equation.Print();
    reducedEquation.Print();
    reducedEquation.SolveIterative();
    
    return 0;
}

