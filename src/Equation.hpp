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
    const size_t N;
    SparseMatrix<float> K;
    vector<float> f;

    Equation(const size_t N);
    pair<unique_ptr<vector<float>>, int> SolveIterative();
    void Print();
    friend ostream& operator<<(ostream& os, const Equation& equation);
};

#endif
