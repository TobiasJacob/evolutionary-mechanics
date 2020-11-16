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
    Matrix<int> cornerIndex; // A unique number for each corner (after calculateIndex)
public:
    const int Rows, Cols; // Size of the field

    Field(int rows, int cols);
    void CalculateIndex(); // Numbers each block
    void Print();
    int GetCounter();


    inline bool &Plane(int r, int c) // Reference for getting or setting a value
    {
        return fields.Value(r, c);
    }

    inline int &CornerIndex(int r, int c) // Reference for getting or setting a value
    {
        return cornerIndex.Value(r, c);
    }
};

#endif