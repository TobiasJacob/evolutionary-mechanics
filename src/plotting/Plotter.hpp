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
public:
    Plotter(string fileName);
    void plot(const Field &field, const vector<float> &u, const Matrix<int> &cornerIndexRow, const Matrix<int> &cornerIndexCol, const Support &supports, const vector<Force> &forces);
    ~Plotter();
};

#endif