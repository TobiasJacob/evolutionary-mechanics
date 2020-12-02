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

public:
    const int Rows, Cols; // Size of the field

    Field(int rows, int cols);

    inline bool &Plane(int r, int c) // Reference for getting or setting a value
    {
        return fields.Value(r, c);
    }

    friend ostream& operator<<(ostream& os, const Field& field);
};


#endif
