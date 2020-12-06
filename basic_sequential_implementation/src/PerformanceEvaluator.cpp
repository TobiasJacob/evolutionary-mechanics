#include "PerformanceEvaluator.hpp"
#include "Equation.hpp"
#include "Matrix.hpp"
#include "Microtime.hpp"
#include <omp.h>
#include <fstream>
#include <algorithm>

// This is the stiffness matrix for a material similar to steel.
const static float K[6][6] = {
    { 300,  180, -240,  -60,  -60, -120},
    { 180,  300, -120,  -60,  -60, -240},
    {-240, -120,  240,    0,    0,  120},
    { -60,  -60,    0,   60,   60,    0},
    { -60,  -60,    0,   60,   60,    0},
    {-120, -240,  120,    0,    0,  240}
};

// This is the deformation matrix for a rectangular triangle
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
    // This adds forces to the equation system. It is easy to parallelize since each force HAS to be (by requirement) on a different position
    for (size_t i = 0; i < forces.size(); i++)
    {
        // Get force i
        const Force &f = forces[i];

        // Get the equation index of the corner the force is attacking on
        size_t forceIndexRow = cornerIndexRow.Value(f.attackCorner.row, f.attackCorner.col);
        // Make sure it is attached to the structure
        #ifdef DEBUG
        if (!forceIndexRow)
        {
            cerr << field << endl;
            cerr << f.attackCorner.row  << ", " << f.attackCorner.col << endl;
            cerr << cornerIndexRow << endl;
            throw std::runtime_error("Force not attached (row)");
        }
        #endif
        // Add row force to the equation system
        equation->f[forceIndexRow - 1] += f.forceRow;

        // Get the equation index of the corner the force is attacking on
        size_t forceIndexCol = cornerIndexCol.Value(f.attackCorner.row, f.attackCorner.col);
        // Make sure it is attached to the structure
        #ifdef DEBUG
        if (!forceIndexCol)
        {
            cerr << field << endl;
            cerr << f.attackCorner.row << ", " << f.attackCorner.col << endl;
            cerr << cornerIndexCol << endl;
            throw std::runtime_error("Force not attached (col)");
        }
        #endif
        // Add col force to the equation system
        equation->f[forceIndexCol - 1] += f.forceCol;
    }

    // This one adds the stiffness values to the global equation system
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
                            omp_set_lock(&(*equationRowLock)[targetRow]);
                            equation->K.GetOrAllocateValue(targetRow, targetIndicesLower[j] - 1) += K[i][j];
                            omp_unset_lock(&(*equationRowLock)[targetRow]);
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
                            const size_t targetRow = targetIndicesUpper[i] - 1;
                            omp_set_lock(&(*equationRowLock)[targetRow]);
                            equation->K.GetOrAllocateValue(targetRow, targetIndicesUpper[j] - 1) += K[i][j];
                            omp_unset_lock(&(*equationRowLock)[targetRow]);
                        }
            }    
}

void PerformanceEvaluator::calculateStress(Field &field, const vector<float> &q)
{
    // For every plane
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

                // Van Mises Equation https://en.wikipedia.org/wiki/Von_Mises_yield_criterion to get total stress
                float squaredStressLower = sigmaLower[0] * sigmaLower[0] + sigmaLower[1] * sigmaLower[1] + sigmaLower[0] * sigmaLower[1] + 3 * sigmaLower[2] * sigmaLower[2];
                float squaredStressUpper = sigmaUpper[0] * sigmaUpper[0] + sigmaUpper[1] * sigmaUpper[1] + sigmaUpper[0] * sigmaUpper[1] + 3 * sigmaUpper[2] * sigmaUpper[2];

                // Save the stress for later
                size_t i = planeIndex.Value(r, c);
                (*stress)[2 * i] = squaredStressLower;
                (*stress)[2 * i + 1] = squaredStressUpper;
            }
}

void PerformanceEvaluator::refreshCornerIndex(Field &field) 
{
    // Helper function to detect if a corner is on a support. These corners simply get removed from the equation system.
    auto cornerRowUnused = [=](size_t r, size_t c) { return any_of(supports.RowSupports.begin(), supports.RowSupports.end(), [=](const Point &p) { return p.row == r && p.col == c; }); };
    auto cornerColUnused = [=](size_t r, size_t c) { return any_of(supports.ColSupports.begin(), supports.ColSupports.end(), [=](const Point &p) { return p.row == r && p.col == c; }); };

    // Reset numbering
    cornerIndexRow.SetTo(0);
    cornerIndexCol.SetTo(0);
    conditions = 0;
    planes = 0;

    // Iterate over all planes
    for (size_t r = 0; r < rows; r++)
        for (size_t c = 0; c < cols; c++)
            if (field.Plane(r, c))
            {
                // Set and increase plane index
                planeIndex.Value(r, c) = planes++;

                // Check each of the four corners in row and col direction. If it has not been assigned an index yet, and is not a support, set and
                // increase the index. Each corner will have a equation associated with it
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

// Field must have connected Planes, and no more than one force per position, since race-condition may occur otherwise. Also, all forces have to be
// connected to the model. In general this function does not perform a rank check, so the field has to be properly defined. The evolutionary algorithm
// will take care that it only generates vaild fields. An invalid field for example would be a single plane without supports and forces. In that moment
// the equation system becomes underdetermined.
float PerformanceEvaluator::GetPerformance(Field &field, optional<string> outputFileName)
{
    // Check that field has correct size
    #ifdef DEBUG
    if (field.Rows != rows || field.Cols != cols)
        throw new exception();
    #endif
    
    // This part cannot be parallelized. Also, the vectors have to be shared memory. It helps therefore to join the threads after this.
    // Update corner index
    refreshCornerIndex(field);

    // Pre-allocate / reset memory
    fTilde = make_unique<vector<float> >(conditions);
    resids = make_unique<vector<float> >(conditions);
    stress = make_unique<vector<float> >(planes * 2);
    residuum = 0;
    maxStress = 0;
    equation = make_unique<Equation>(conditions);
    equationRowLock = make_unique<vector<omp_lock_t> >(conditions);

    // Start timer
    double start = microtime();

    // Spawn threads to solve equation in parallel
    {
        // Generates an equation system from the field / mesh
        setupEquation(field);
        // Solve equation
        equation->SolveIterative();

        // Calculate the residuum to validate accuracy of the solution
        equation->K.Multiply(equation->GetSolution(), *fTilde);
        subtract(*fTilde, equation->f, *resids);
        l2square(*resids, residuum);

        // Calculate stress
        calculateStress(field, equation->GetSolution());

        // Get the maximum stress
        for (size_t i = 0; i < planes * 2; i++)
            maxStress = max(maxStress, (*stress)[i]);
    }

    // Stop timer
    lastSolvingTime = microtime() - start;

    // Maybe put out debug view
    if (outputFileName.has_value())
    {
        // This class will take care of generating the html file with all the information.
        Plotter plotter(*outputFileName);
        plotter.plot(field, equation->GetSolution(), cornerIndexRow, cornerIndexCol, supports, forces, equation->GetSteps(), residuum, *stress, lastSolvingTime);
    }

    // Return maximum stress
    return maxStress;
}

size_t PerformanceEvaluator::GetConditions() 
{
    return conditions;
}

double PerformanceEvaluator::GetLastSolvingTime() 
{
    return lastSolvingTime;
}
