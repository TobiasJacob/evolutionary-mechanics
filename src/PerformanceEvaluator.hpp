#ifndef PERFORMANCEEVALUATOR
#define PERFORMANCEEVALUATOR

#include <vector>
#include <optional>
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
    Matrix<size_t> cornerIndexRow; // A unique number for each corner for force in row direction
    Matrix<size_t> cornerIndexCol; // A unique number for each corner for force in col direction

    Equation setupEquation(Field &field);
    vector<float> calculateStress(Field &field, const vector<float> &q);
    void refreshCornerIndex(Field &field);
    bool isUnused(size_t equationRow);
public:
    PerformanceEvaluator(const size_t rows, const size_t cols, const Support &supports, const vector<Force> &forces);
    float GetPerformance(Field &field, optional<string> outputFileName);
};

#endif
