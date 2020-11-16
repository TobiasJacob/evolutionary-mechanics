#ifndef EQUATION
#define EQUATION

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <math.h>
#include <iomanip>
#include "Matrix.hpp"

using namespace std;

struct Equation
{
    const int N;
    Matrix<float> K;
    vector<float> f;

    Equation(const int N);
    unique_ptr<vector<float>> SolveIterative();
    void Print();
};

#endif