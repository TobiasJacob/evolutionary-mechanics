#include "PerformanceEvaluator.hpp"
#include "Equation.hpp"
#include "Matrix.hpp"
#include "Microtime.hpp"
#include <fstream>
#include <algorithm>

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

PerformanceEvaluator::PerformanceEvaluator(const int rows, const int cols, const Support &supports, const vector<Force> &forces) : rows(rows), cols(cols), supports(supports), forces(forces), conditions(0), cornerIndexRow(rows + 1, cols + 1, 0), cornerIndexCol(rows + 1, cols + 1, 0)
{
    
}


Equation PerformanceEvaluator::setupEquation(Field &field) 
{
    // Add three constrains for global position and rotation
    Equation equation(conditions);

    // Add forces to equation. If force is not attached to model, model failed.
    // Forces on supports are automatically removed later
    for (const Force &f: forces)
    {
        int forceIndexRow = cornerIndexRow.Value(f.attackCorner.row, f.attackCorner.col);
        if (!forceIndexRow)
        {
            // cout << f.attackCorner.row << ", " << f.attackCorner.col << " out of bounds" << endl;
            throw INFINITY;
        }
        equation.f[forceIndexRow - 1] += f.forceRow;
        int forceIndexCol = cornerIndexCol.Value(f.attackCorner.row, f.attackCorner.col);
        if (!forceIndexCol)
        {
            // cout << f.attackCorner.row << ", " << f.attackCorner.col << " out of bounds" << endl;
            throw INFINITY;
        }
        equation.f[forceIndexCol - 1] += f.forceCol;
    }

    // Setup stiffness matrix
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            if (field.Plane(r, c))
            {
                // Lower left triangle, get the index of each corner in the global equation system
                const int targetIndicesLower[6] = {
                    cornerIndexRow.Value(r, c),
                    cornerIndexCol.Value(r, c),
                    cornerIndexRow.Value(r + 1, c),
                    cornerIndexCol.Value(r + 1, c),
                    cornerIndexRow.Value(r, c + 1),
                    cornerIndexCol.Value(r, c + 1)
                };

                // Add the stiffness values to global equation
                for (int i = 0; i < 6; i++)
                    for (int j = 0; j < 6; j++)
                        if (targetIndicesLower[i] && targetIndicesLower[j])
                            equation.K.GetOrAllocateValue(targetIndicesLower[i] - 1, targetIndicesLower[j] - 1) += K[i][j];

                // Upper right triangle, get the index of each corner in the global equation system
                const int targetIndicesUpper[6] = {
                    cornerIndexRow.Value(r + 1, c + 1),
                    cornerIndexCol.Value(r + 1, c + 1),
                    cornerIndexRow.Value(r, c + 1),
                    cornerIndexCol.Value(r, c + 1),
                    cornerIndexRow.Value(r + 1, c),
                    cornerIndexCol.Value(r + 1, c)
                };

                // Add the stiffness values to global equation
                for (int i = 0; i < 6; i++)
                    for (int j = 0; j < 6; j++)
                        if (targetIndicesUpper[i] && targetIndicesUpper[j])
                            equation.K.GetOrAllocateValue(targetIndicesUpper[i] - 1, targetIndicesUpper[j] - 1) += K[i][j];
            }
    
    return equation;
}

vector<float> PerformanceEvaluator::calculateStress(Field &field, const vector<float> &q) 
{
    int planes = 0;
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            if (field.Plane(r, c))
                planes++;
    vector<float> stress(planes * 2);
    // For every tile
    int i = 0;
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            if (field.Plane(r, c))
            {
                // Lower left triangle, get the index of each corner in the global equation system
                const int targetIndicesLower[6] = {
                    cornerIndexRow.Value(r, c),
                    cornerIndexCol.Value(r, c),
                    cornerIndexRow.Value(r + 1, c),
                    cornerIndexCol.Value(r + 1, c),
                    cornerIndexRow.Value(r, c + 1),
                    cornerIndexCol.Value(r, c + 1)
                };
                
                // Store stress
                float sigmaLower[3] = {0, 0, 0};

                // Calculate compressive and shear stress
                for (int i = 0; i < 3; i++)
                    for (int j = 0; j < 6; j++)
                        if (targetIndicesLower[j])
                            sigmaLower[i] += ETimesB[i][j] * q[targetIndicesLower[j] - 1];

                // Upper right triangle, get the index of each corner in the global equation system
                const int targetIndicesUpper[6] = {
                    cornerIndexRow.Value(r + 1, c + 1),
                    cornerIndexCol.Value(r + 1, c + 1),
                    cornerIndexRow.Value(r, c + 1),
                    cornerIndexCol.Value(r, c + 1),
                    cornerIndexRow.Value(r + 1, c),
                    cornerIndexCol.Value(r + 1, c)
                };

                // Store stress
                float sigmaUpper[3] = {0, 0, 0};

                // Calculate compressive and shear stress
                for (int i = 0; i < 3; i++)
                    for (int j = 0; j < 6; j++)
                        if (targetIndicesUpper[j])
                            sigmaUpper[i] += ETimesB[i][j] * q[targetIndicesUpper[j] - 1];

                // Van Mises Equation https://en.wikipedia.org/wiki/Von_Mises_yield_criterion
                float squaredStressLower = sigmaLower[0] * sigmaLower[0] + sigmaLower[1] * sigmaLower[1] + sigmaLower[0] * sigmaLower[1] + 3 * sigmaLower[2] * sigmaLower[2];
                float squaredStressUpper = sigmaUpper[0] * sigmaUpper[0] + sigmaUpper[1] * sigmaUpper[1] + sigmaUpper[0] * sigmaUpper[1] + 3 * sigmaUpper[2] * sigmaUpper[2];
                stress[i++] = squaredStressLower;
                stress[i++] = squaredStressUpper;
            }
    return stress;
}

void PerformanceEvaluator::refreshCornerIndex(Field &field) 
{
    auto cornerRowUnused = [=](int r, int c) { return any_of(supports.RowSupports.begin(), supports.RowSupports.end(), [=](const Point &p) { return p.row == r && p.col == c; }); };
    auto cornerColUnused = [=](int r, int c) { return any_of(supports.ColSupports.begin(), supports.ColSupports.end(), [=](const Point &p) { return p.row == r && p.col == c; }); };

    cornerIndexRow.SetTo(0);
    cornerIndexCol.SetTo(0);
    conditions = 0;
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            if (field.Plane(r, c))
            {
                if (!cornerIndexRow.Value(r, c) && !cornerRowUnused(r, c)) cornerIndexRow.Value(r, c) = ++conditions;
                if (!cornerIndexCol.Value(r, c) && !cornerColUnused(r, c)) cornerIndexCol.Value(r, c) = ++conditions;
                if (!cornerIndexRow.Value(r + 1, c) && !cornerRowUnused(r + 1, c)) cornerIndexRow.Value(r + 1, c) = ++conditions;
                if (!cornerIndexCol.Value(r + 1, c) && !cornerColUnused(r + 1, c)) cornerIndexCol.Value(r + 1, c) = ++conditions;
                if (!cornerIndexCol.Value(r + 1, c + 1) && !cornerColUnused(r + 1, c + 1)) cornerIndexCol.Value(r + 1, c + 1) = ++conditions;
                if (!cornerIndexRow.Value(r + 1, c + 1) && !cornerRowUnused(r + 1, c + 1)) cornerIndexRow.Value(r + 1, c + 1) = ++conditions;
                if (!cornerIndexRow.Value(r, c + 1) && !cornerRowUnused(r, c + 1)) cornerIndexRow.Value(r, c + 1) = ++conditions;
                if (!cornerIndexCol.Value(r, c + 1) && !cornerColUnused(r, c + 1)) cornerIndexCol.Value(r, c + 1) = ++conditions;

            }
}

float PerformanceEvaluator::GetPerformance(Field &field, optional<string> outputFileName)
{
    // TODO: Check that structure is connected, and that supports are connected and row supports not in same column
    if (field.Rows != rows || field.Cols != cols)
        throw new exception();
    // Get corner numbering, so each corner of a tile has a determined index, starting at 1
    refreshCornerIndex(field);
    
    try
    {
        double start = microtime();
        // Generates an equation system from the field / mesh
        Equation equation = setupEquation(field);
        
        // Solve equation
        pair<unique_ptr<vector<float>>, int> solution = equation.SolveIterative();
        double stop = microtime();

        // Calculate residum
        vector<float> fTilde = equation.K * *(solution.first);
        vector<float> resids = subtract(fTilde, equation.f);
        float residuum = l2square(resids);

        // Calculate maximum stress
        vector<float> stress = calculateStress(field, *solution.first);
        float maxStress = *max_element(stress.begin(), stress.end());

        // Maybe put out debug view
        if (outputFileName.has_value())
        {
            Plotter plotter(*outputFileName);
            plotter.plot(field, *solution.first, cornerIndexRow, cornerIndexCol, supports, forces, solution.second, residuum, stress, stop - start); // steps, residum, sigma
        }

        return maxStress;
    }
    catch(const float val)
    {
        return INFINITY;
    }
}
