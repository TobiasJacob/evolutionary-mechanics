#ifndef FIELD
#define FIELD

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include "Matrix.hpp"
#include "Equation.hpp"

using namespace std;

struct Support
{
    int row;
    int col;
};

struct Force
{
    int row;
    int col;
    float forceRow;
    float forceCol;
};

class Field
{
private:
    const int rows, cols;
    Matrix<bool> field;
    vector<Support> supports;
    vector<Force> forces;
    int counter;
    Matrix<int> indexed;
public:
    Field(int rows, int cols);
    void calculateIndex();
    unique_ptr<Equation> buildEquation();
    double calculatePerformance();
    void Print();
};

#endif