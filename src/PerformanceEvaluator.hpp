#ifndef PERFORMANCEEVALUATOR
#define PERFORMANCEEVALUATOR

#include <vector>
#include <optional>
#include <memory>
#include "Field.hpp"
#include "plotting/Plotter.hpp"
#include "VectorOperations.hpp"

class PerformanceEvaluator
{
private:
    const size_t rows, cols;
    const Support &supports;
    const vector<Force> &forces;

    size_t conditions; // The maximum number
    size_t planes;
    Matrix<size_t> cornerIndexRow; // A unique number for each corner for force in row direction
    Matrix<size_t> cornerIndexCol; // A unique number for each corner for force in col direction
    Matrix<size_t> planeIndex; // A unique number for each corner for force in col direction

    float residuum = 0;
    float maxStress = 0;

    // Used in the parallel GetPerformance
    unique_ptr<vector<float> > fTilde;
    unique_ptr<vector<float> > resids;
    unique_ptr<vector<float> > stress;

    Equation setupEquation(Field &field);
    void calculateStress(Field &field, const vector<float> &q, vector<float> &stress);
    void refreshCornerIndex(Field &field);
    bool isUnused(size_t equationRow);
public:
    PerformanceEvaluator(const size_t rows, const size_t cols, const Support &supports, const vector<Force> &forces);
    float GetPerformance(Field &field, optional<string> outputFileName);
};

#endif
