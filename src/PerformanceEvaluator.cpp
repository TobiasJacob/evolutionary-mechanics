#include "PerformanceEvaluator.hpp"
#include "equation.hpp"
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

PerformanceEvaluator::PerformanceEvaluator(const size_t rows, const size_t cols, const Support &supports, const vector<Force> &forces)
    : rows(rows), cols(cols), supports(supports), forces(forces), conditions(0)
    , cornerIndexRow(rows + 1, cols + 1, 0)
    , cornerIndexCol(rows + 1, cols + 1, 0)
    , planeIndex(rows, cols, 0)
{
    
}


void PerformanceEvaluator::setupEquation(Field &field)
{
    // Add forces to equation-> If force is not attached to model, model failed.
    // Forces on supports are not set
    // This one is easy to parallelize since each force HAS to be (by requirement) on a different position
    //#pragma omp for schedule(static, 1)
    #pragma omp single
    for (const Force &f: forces)
    {
        size_t forceIndexRow = cornerIndexRow.Value(f.attackCorner.row, f.attackCorner.col);
        equation->f[forceIndexRow - 1] += f.forceRow;
        size_t forceIndexCol = cornerIndexCol.Value(f.attackCorner.row, f.attackCorner.col);
        equation->f[forceIndexCol - 1] += f.forceCol;
    }

    //#pragma omp for schedule(static, 16)
    #pragma omp single
    for (size_t r = 0; r < rows; r++)
        for (size_t c = 0; c < cols; c++)
            if (field.Plane(r, c))
            {
                // Lower left triangle, get the index of each corner in the global equation system
                const size_t targetIndicesLower[6] = {
                    cornerIndexRow.Value(r, c),
                    cornerIndexCol.Value(r, c),
                    cornerIndexRow.Value(r + 1, c),
                    cornerIndexCol.Value(r + 1, c),
                    cornerIndexRow.Value(r, c + 1),
                    cornerIndexCol.Value(r, c + 1)
                };

                // Add the stiffness values to global equation
                for (size_t i = 0; i < 6; i++)
                    for (size_t j = 0; j < 6; j++)
                        if (targetIndicesLower[i] && targetIndicesLower[j])
                        {
                            const size_t targetRow = targetIndicesLower[i] - 1;
                            //omp_set_lock(&(*equationRowLock)[targetRow]);
                            equation->K.GetOrAllocateValue(targetRow, targetIndicesLower[j] - 1) += K[i][j];
                            //omp_unset_lock(&(*equationRowLock)[targetRow]);
                        }

                // Upper right triangle, get the index of each corner in the global equation system
                const size_t targetIndicesUpper[6] = {
                    cornerIndexRow.Value(r + 1, c + 1),
                    cornerIndexCol.Value(r + 1, c + 1),
                    cornerIndexRow.Value(r, c + 1),
                    cornerIndexCol.Value(r, c + 1),
                    cornerIndexRow.Value(r + 1, c),
                    cornerIndexCol.Value(r + 1, c)
                };

                // Add the stiffness values to global equation
                for (size_t i = 0; i < 6; i++)
                    for (size_t j = 0; j < 6; j++)
                        if (targetIndicesUpper[i] && targetIndicesUpper[j])
                        {
                            const size_t targetRow = targetIndicesLower[i] - 1;
                            //omp_set_lock(&(*equationRowLock)[targetRow]);
                            equation->K.GetOrAllocateValue(targetRow, targetIndicesLower[j] - 1) += K[i][j];
                            //omp_unset_lock(&(*equationRowLock)[targetRow]);
                        }
            }    
}

void PerformanceEvaluator::calculateStress(Field &field, const vector<float> &q, vector<float> &stress)
{
    // For every tile
    #pragma omp for schedule(static, 16)
    for (size_t r = 0; r < rows; r++)
        for (size_t c = 0; c < cols; c++)
            if (field.Plane(r, c))
            {
                // Lower left triangle, get the index of each corner in the global equation system
                const size_t targetIndicesLower[6] = {
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
                for (size_t i = 0; i < 3; i++)
                    for (size_t j = 0; j < 6; j++)
                        if (targetIndicesLower[j])
                            sigmaLower[i] += ETimesB[i][j] * q[targetIndicesLower[j] - 1];

                // Upper right triangle, get the index of each corner in the global equation system
                const size_t targetIndicesUpper[6] = {
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
                for (size_t i = 0; i < 3; i++)
                    for (size_t j = 0; j < 6; j++)
                        if (targetIndicesUpper[j])
                            sigmaUpper[i] += ETimesB[i][j] * q[targetIndicesUpper[j] - 1];

                // Van Mises Equation https://en.wikipedia.org/wiki/Von_Mises_yield_criterion
                float squaredStressLower = sigmaLower[0] * sigmaLower[0] + sigmaLower[1] * sigmaLower[1] + sigmaLower[0] * sigmaLower[1] + 3 * sigmaLower[2] * sigmaLower[2];
                float squaredStressUpper = sigmaUpper[0] * sigmaUpper[0] + sigmaUpper[1] * sigmaUpper[1] + sigmaUpper[0] * sigmaUpper[1] + 3 * sigmaUpper[2] * sigmaUpper[2];
                size_t i = planeIndex.Value(r, c);
                stress[2 * i] = squaredStressLower;
                stress[2 * i + 1] = squaredStressUpper;
            }
}

void PerformanceEvaluator::refreshCornerIndex(Field &field) 
{
    auto cornerRowUnused = [=](size_t r, size_t c) { return any_of(supports.RowSupports.begin(), supports.RowSupports.end(), [=](const Point &p) { return p.row == r && p.col == c; }); };
    auto cornerColUnused = [=](size_t r, size_t c) { return any_of(supports.ColSupports.begin(), supports.ColSupports.end(), [=](const Point &p) { return p.row == r && p.col == c; }); };

    cornerIndexRow.SetTo(0);
    cornerIndexCol.SetTo(0);
    conditions = 0;
    planes = 0;

    for (size_t r = 0; r < rows; r++)
        for (size_t c = 0; c < cols; c++)
            if (field.Plane(r, c))
            {
                planeIndex.Value(r, c) = planes++;
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

// Requirements: Field must have connected Planes, and no more than one force per position, since race-condition may occur otherwise. Also, all forces have to be connected to the model.
float PerformanceEvaluator::GetPerformance(Field &field, optional<string> outputFileName)
{
    // TODO: Check that structure is connected, and that supports are connected and row supports not in same column
    if (field.Rows != rows || field.Cols != cols)
        throw new exception();
    
    #pragma omp single
    {
        // Get corner numbering, so each corner of a tile has a determined index, starting at 1
        refreshCornerIndex(field);
        fTilde = make_unique<vector<float> >(conditions);
        resids = make_unique<vector<float> >(conditions);
        stress = make_unique<vector<float> >(planes * 2);
        residuum = 0;
        maxStress = 0;
        equation = make_unique<Equation>(conditions);
        equationRowLock = make_unique<vector<omp_lock_t> >(conditions);
    }
    #pragma omp barrier

    // Calculate residum
    double start = microtime();
    // Generates an equation system from the field / mesh
    setupEquation(field);

    #pragma omp parallel
    {
        // Solve equation
        equation->SolveIterative();

        equation->K.Multiply(equation->GetSolution(), *fTilde);
        subtract(*fTilde, equation->f, *resids);
        l2square(*resids, residuum);

        // Calculate maximum stress
        calculateStress(field, equation->GetSolution(), *stress);
        #pragma omp for reduction(max:maxStress) schedule(static, 256)
        for (size_t i = 0; i < planes * 2; i++)
            maxStress = max(maxStress, (*stress)[i]);
    }

    double stop = microtime();

    // Maybe put out debug view
    #pragma omp single
    if (outputFileName.has_value())
    {
        Plotter plotter(*outputFileName);
        plotter.plot(field, equation->GetSolution(), cornerIndexRow, cornerIndexCol, supports, forces, equation->GetSteps(), residuum, *stress, stop - start); // steps, residum, sigma
    }
    #pragma omp barrier
    return maxStress;
}
