#ifndef PERFORMANCEEVALUATOR
#define PERFORMANCEEVALUATOR

#include <vector>
#include <optional>
#include <memory>
#include <omp.h>
#include "Field.hpp"
#include "plotting/Plotter.hpp"
#include "VectorOperations.hpp"

/**
 * @brief This class takes care of evaulating the performance of a mechanical structure.
 * 
 */
class PerformanceEvaluator
{
private:
    const size_t rows, cols; // Size of the field
    const Support &supports; // Anchor points of the mechanical structure
    const vector<Force> &forces; // The forces attacking the mechanical structure

    size_t conditions; // The number of equations to solve
    size_t planes; // How many planes the mechanical structure has
    Matrix<size_t> cornerIndexRow; // A unique number for each corner for row direction displacement and force
    Matrix<size_t> cornerIndexCol; // A unique number for each corner for col direction displacement and force
    Matrix<size_t> planeIndex; // A unique number for each plane

    float residuum = 0; // The error of the found solution
    float maxStress = 0; // Maximum stress observed on the structure. Will indicate if it breaks.

    // Used in the parallel GetPerformance
    unique_ptr<vector<float> > fTilde; // Vector to store the solutions approximation
    unique_ptr<vector<float> > resids; // Vector to calculate the residums (errors)
    unique_ptr<vector<float> > stress; // Vector to store the stress
    
    unique_ptr<Equation> equation; // The equation system
    unique_ptr<vector<omp_lock_t> > equationRowLock; // A vector of locks for parallel row accessing

    double lastSolvingTime = 0; // Stores the time required for the last solve

    /**
     * @brief Internal function, sets up the equation system. Has to be called after refreshCornerIndex
     * 
     * @param field Describing the mechanical structure
     */
    void setupEquation(Field &field);

    /**
     * @brief Converts the displacement 
     * 
     * @param field 
     * @param q 
     * @param stress 
     */
    void calculateStress(Field &field, const vector<float> &q);
    void refreshCornerIndex(Field &field);
    bool isUnused(size_t equationRow);
public:
    PerformanceEvaluator(const size_t rows, const size_t cols, const Support &supports, const vector<Force> &forces);
    float GetPerformance(Field &field, optional<string> outputFileName);
    size_t GetConditions();
    double GetLastSolvingTime();
};

#endif
