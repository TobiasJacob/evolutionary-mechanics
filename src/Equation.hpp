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
    int Conditions;
    int Unknowns;
    Matrix<float> K;
    vector<float> f;

    Equation(int unknowns, int conditions);
    unique_ptr<vector<float>> SolveIterative();
    void Print();
};

#endif