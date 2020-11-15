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
    int N;
    Matrix<float> K;
    vector<float> f;

    Equation(int N);
    unique_ptr<vector<float>> solveIterative();
    void print();
};

#endif