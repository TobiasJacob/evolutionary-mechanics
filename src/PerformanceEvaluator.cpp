#include "PerformanceEvaluator.hpp"
#include "Equation.hpp"
#include "Matrix.hpp"

const static float K[6][6] = {
    { 300,  180, -240,  -60,  -60, -120},
    { 180,  300, -120,  -60,  -60, -240},
    {-240, -120,  240,    0,    0,  120},
    { -60,  -60,    0,   60,   60,    0},
    { -60,  -60,    0,   60,   60,    0},
    {-120, -240,  120,    0,    0,  240}
};

const static float ETimesB[3][6] = {
    {-240, -120,  240,    0,    0,  120},
    {-120, -240,  120,    0,    0,  240},
    {-60,  -60,    0,   60,   60,    0}
};

Equation PerformanceEvaluator::SetupEquation(Field &field, const Support &supports, const vector<Force> forces) 
{
    // Add three constrains for global position and rotation
    const int conditions = 2 * field.GetCounter();
    Equation equation(conditions);

    // Add forces to equation. If force is not attached to model, model failed.
    // Forces on supports are automatically removed later
    for (const Force &f: forces)
    {
        int cornerIndex = field.CornerIndex(f.attackCorner.row, f.attackCorner.col);
        if (!cornerIndex) throw INFINITY;
        equation.f[2 * cornerIndex - 2] += f.forceRow;
        equation.f[2 * cornerIndex - 1] += f.forceCol;
    }

    // Setup stiffness matrix
    for (int r = 0; r < field.Rows; r++)
        for (int c = 0; c < field.Cols; c++)
            if (field.Plane(r, c))
            {
                // Lower left triangle, get the index of each corner in the global equation system
                const int targetIndicesLower[6] = {
                    2 * field.CornerIndex(r, c) - 2,
                    2 * field.CornerIndex(r, c) - 1,
                    2 * field.CornerIndex(r + 1, c) - 2,
                    2 * field.CornerIndex(r + 1, c) - 1,
                    2 * field.CornerIndex(r, c + 1) - 2,
                    2 * field.CornerIndex(r, c + 1) - 1
                };

                // Add the stiffness values to global equation
                for (int i = 0; i < 6; i++)
                    for (int j = 0; j < 6; j++)
                        equation.K.Value(targetIndicesLower[i], targetIndicesLower[j]) += K[i][j];

                // Upper right triangle, get the index of each corner in the global equation system
                const int targetIndicesUpper[6] = {
                    2 * field.CornerIndex(r + 1, c + 1) - 2,
                    2 * field.CornerIndex(r + 1, c + 1) - 1,
                    2 * field.CornerIndex(r, c + 1) - 2,
                    2 * field.CornerIndex(r, c + 1) - 1,
                    2 * field.CornerIndex(r + 1, c) - 2,
                    2 * field.CornerIndex(r + 1, c) - 1
                };

                // Add the stiffness values to global equation
                for (int i = 0; i < 6; i++)
                    for (int j = 0; j < 6; j++)
                        equation.K.Value(targetIndicesUpper[i], targetIndicesUpper[j]) += K[i][j];
            }
    
    return equation;
}

float PerformanceEvaluator::calculateMaxStress(Field &field, const vector<float> &q) 
{
    float maxStressSquared = 0;
    // For every tile
    for (int r = 0; r < field.Rows; r++)
        for (int c = 0; c < field.Cols; c++)
            if (field.Plane(r, c))
            {
                // Lower left triangle, get the index of each corner in the global equation system
                const int targetIndicesLower[6] = {
                    2 * field.CornerIndex(r, c) - 2,
                    2 * field.CornerIndex(r, c) - 1,
                    2 * field.CornerIndex(r + 1, c) - 2,
                    2 * field.CornerIndex(r + 1, c) - 1,
                    2 * field.CornerIndex(r, c + 1) - 2,
                    2 * field.CornerIndex(r, c + 1) - 1
                };
                
                // Store stress
                float sigmaLower[3] = {0, 0, 0};

                // Calculate compressive and shear stress
                for (int i = 0; i < 3; i++)
                    for (int j = 0; j < 6; j++)
                        sigmaLower[i] += ETimesB[i][j] * q[targetIndicesLower[j]];

                // Upper right triangle, get the index of each corner in the global equation system
                const int targetIndicesUpper[6] = {
                    2 * field.CornerIndex(r + 1, c + 1) - 2,
                    2 * field.CornerIndex(r + 1, c + 1) - 1,
                    2 * field.CornerIndex(r, c + 1) - 2,
                    2 * field.CornerIndex(r, c + 1) - 1,
                    2 * field.CornerIndex(r + 1, c) - 2,
                    2 * field.CornerIndex(r + 1, c) - 1
                };

                // Store stress
                float sigmaUpper[3] = {0, 0, 0};

                // Calculate compressive and shear stress
                for (int i = 0; i < 3; i++)
                    for (int j = 0; j < 6; j++)
                        sigmaUpper[i] += ETimesB[i][j] * q[targetIndicesUpper[j]];

                // Van Mises Equation https://en.wikipedia.org/wiki/Von_Mises_yield_criterion
                float squaredStressLower = sigmaLower[0] * sigmaLower[0] + sigmaLower[1] * sigmaLower[1] + sigmaLower[0] * sigmaLower[1] + 3 * sigmaLower[2] * sigmaLower[2];
                float squaredStressUpper = sigmaUpper[0] * sigmaUpper[0] + sigmaUpper[1] * sigmaUpper[1] + sigmaUpper[0] * sigmaUpper[1] + 3 * sigmaUpper[2] * sigmaUpper[2];
                if (squaredStressLower > maxStressSquared) maxStressSquared = squaredStressLower;
                if (squaredStressUpper > maxStressSquared) maxStressSquared = squaredStressUpper;
            }
    return maxStressSquared;
}

float PerformanceEvaluator::GetPerformance(Field &field, const Support &supports, const vector<Force> forces)
{
    // Refresh corner numbering, so each corner of a tile has a determined index, starting at 1
    field.CalculateIndex();
    const int conditions = 2 * field.GetCounter();
    // Set up equations for the supports. If one support is not attached to the field, field failed.
    // Supports Q is set to fixed zero.
    // Support values must be different from each other.
    int supportColIndex = 2 * field.CornerIndex(supports.SupportCol.row, supports.SupportCol.col) - 1;
    int supportRow1Index = 2 * field.CornerIndex(supports.SupportRow1.row, supports.SupportRow1.col) - 2;
    int supportRow2Index = 2 * field.CornerIndex(supports.SupportRow2.row, supports.SupportRow2.col) - 2;

    // Make sure support index are valid, especially the row supports are different
    if (supportColIndex < 0 || supportRow1Index < 0 || supportColIndex < 0 || supportRow1Index == supportRow2Index)
        return INFINITY;

    try
    {
        // Generates an equation system from the field / mesh
        Equation equation = SetupEquation(field, supports, forces);
        Equation reducedEquation(conditions - 3);

        // The values of the supports are zero, and can therefore be removed.
        int rTarget = 0;
        for (int r = 0; r < conditions; r++)
        {
            if (r == supportColIndex || r == supportRow1Index || r == supportRow2Index)
                continue;
            reducedEquation.f[rTarget] = equation.f[r];
            int cTarget = 0;
            for (int c = 0; c < conditions; c++)
            {
                if (c == supportColIndex || c == supportRow1Index || c == supportRow2Index)
                    continue;
                reducedEquation.K.Value(rTarget, cTarget) = equation.K.Value(r, c);
                cTarget++;
            }
            rTarget++;
        }
        
        // Solve equation
        pair<unique_ptr<vector<float>>, int> solution = reducedEquation.SolveIterative();

        // Calculate residum
        vector<float> fTilde = reducedEquation.K * *(solution.first);
        vector<float> residuum = subtract(fTilde, reducedEquation.f);
        cout << "Solved after " << solution.second << " steps with residuum " << l2square(residuum) << endl;
        
        // Add the values of the supports again, which are zero
        vector<float> q(conditions, 0);
        int rSource = 0;
        for (int r = 0; r < conditions; r++)
        {
            if (r == supportColIndex || r == supportRow1Index || r == supportRow2Index)
                continue;
            q[r] = (*(solution.first))[rSource];
            rSource++;
        }

        // Calculate maximum stress
        float maxStress = calculateMaxStress(field, q);
        return maxStress;
    }
    catch(const float val)
    {
        return INFINITY;
    }
}


