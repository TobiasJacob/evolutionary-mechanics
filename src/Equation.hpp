#ifndef EQUATION
#define EQUATION

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <math.h>
#include <iomanip>
#include "Matrix.hpp"
#include "SparseMatrix.hpp"
#include "Microtime.hpp"

using namespace std;

struct Equation
{
private:
    unique_ptr<vector<float>> x_k;
    unique_ptr<vector<float>> r_k;
    unique_ptr<vector<float>> p_k;

    unique_ptr<vector<float>> x_k1;
    unique_ptr<vector<float>> r_k1;
    unique_ptr<vector<float>> p_k1;

    vector<float> kTimesP;
    vector<float> kTimesx_k;
    vector<float> scaledP_K;
    vector<float> scaledKTimesP;
    float r_k1_squared = 0;
    float alpha_k_divider = 0;
    float alpha_k = 0;

    size_t counter = 0;
    const size_t maxSteps = 10000;
public:
    const size_t N;

    SparseMatrix<float> K;
    vector<float> f;

    Equation(const size_t N);
    void SolveIterative();
    vector<float> &GetSolution();
    int GetSteps();
    void Print();
    friend ostream& operator<<(ostream& os, const Equation& equation);
};

#endif
