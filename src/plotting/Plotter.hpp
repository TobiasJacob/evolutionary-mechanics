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
    void plot(Field &field, vector<float> &u, Matrix<int> &cornerIndexRow, Matrix<int> &cornerIndexCol);
    ~Plotter();
};

#endif