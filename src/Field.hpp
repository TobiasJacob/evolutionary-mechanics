#ifndef FIELD
#define FIELD

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include "Matrix.hpp"
#include "Equation.hpp"

using namespace std;

class Field
{
private:
    const int rows, cols;
    Matrix<bool> field;
    int counter;
    Matrix<int> indexed;
public:
    Field(int rows, int cols);
    void calculateIndex();
    void Print();
};

#endif