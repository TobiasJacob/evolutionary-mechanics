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
 * @brief This class takes care of evaulating the performance of a mechanical structure. It needs the supports that the mechanical structure can attach to
 * and the forces that act on it. It provides a single function then, GetPerformance, which returns the maximum stress observed on a certain field. If it
 * is above a certain threshold, it means that the structure broke.
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
     * @brief Internal function, sets up the equation system that has to be solved depending on the mechanical structure. Has to be called after refreshCornerIndex
     * 
     * @param field Describing the mechanical structure
     */
    void setupEquation(Field &field);

    /**
     * @brief Calculates the stress on each triangle using the displacements.
     * 
     * @param field The mechanical structure
     * @param q The displacements
     */
    void calculateStress(Field &field, const vector<float> &q);

    /**
     * @brief Numerates the corners and planes, so that each has a unique identifier.
     * 
     * @param field The mechanical structure
     */
    void refreshCornerIndex(Field &field);
public:
    /**
     * @brief Construct a new Performance Evaluator object for a certain situation, including the supports that the mechanical structure can attach to
     * and the forces that act on it. It provides 
     * 
     * @param rows The rows of the field that GetPerformance will be called with
     * @param cols The cols of the field that GetPerformance will be called with
     * @param supports All mechanical supports the structure can attach to
     * @param forces The forces that act on the structure
     */
    PerformanceEvaluator(const size_t rows, const size_t cols, const Support &supports, const vector<Force> &forces);

    /**
     * @brief Calculates the maximum stress observed on the field, using the load specified by supports and forces
     * 
     * @param field The mechanical structure
     * @param outputFileName An optional file to save debug output. It creates a html file that can be opend with any arbitrary browser to view the 
     * mechanical structure and results of the evaluation.
     * @return float The maximum stress observed. If it is above a certain threshold, the structure broke.
     */
    float GetPerformance(Field &field, optional<string> outputFileName);

    /**
     * @brief Get the number of equations
     * 
     * @return size_t the number of equations
     */
    size_t GetConditions();

    /**
     * @brief Get the time needed by the last call to GetPerformance
     * 
     * @return double The time needed by the last call to GetPerformance
     */
    double GetLastSolvingTime();
};

#endif
