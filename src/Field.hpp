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
    Matrix<bool> fields; // Contains the bitvalues of which block is set

    int counter; // The maximum number
    Matrix<int> indexed; // A unique number for each block (after calculateIndex)
public:
    const int Rows, Cols; // Size of the field

    Field(int rows, int cols);
    void calculateIndex(); // Numbers each block
    void Print();


    inline bool &field(int r, int c) // Reference for getting or setting a value
    {
        #ifdef DEBUG
        if (r >= Rows || c >= Cols) cerr << "Index access error (" << r << "," << c << ") out of (" << Rows << "," << Cols << ")" << endl;
        #endif
        return fields.value(r, c);
    }
};

#endif