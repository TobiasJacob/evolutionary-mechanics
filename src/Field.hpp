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
    Matrix<bool> field;
    int counter;
    Matrix<int> indexed;
public:
    const int Rows, Cols;

    Field(int rows, int cols);
    void calculateIndex();
    void Print();


    inline bool &block(int r, int c) 
    {
        #ifdef DEBUG
        if (r >= Rows || c >= Cols) cerr << "Index access error (" << r << "," << c << ") out of (" << Rows << "," << Cols << ")" << endl;
        #endif
        return field.value(r, c);
    }
};

#endif