#ifndef PLOTTER
#define PLOTTER

#include <iostream>
#include <fstream>
#include <string>

#include "../Equation.hpp"
#include "../Field.hpp"

using namespace std;

class Plotter
{
private:
    ofstream output;
    string head;
    string tail;
public:
    Plotter(string fileName);
    void plot(const Field &field, const vector<float> &u, const Matrix<size_t> &cornerIndexRow, const Matrix<size_t> &cornerIndexCol, const Support &supports, const vector<Force> &forces, const size_t solutionSteps, const float residum, const vector<float> &stress, float time);
    ~Plotter();
};

#endif