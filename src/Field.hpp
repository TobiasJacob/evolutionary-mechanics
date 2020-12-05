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
    const size_t Rows, Cols; // Size of the field

    Field(size_t rows, size_t cols, bool defaultValue = false);

    inline bool &Plane(size_t r, size_t c) // Reference for getting or setting a value
    {
        return fields.Value(r, c);
    }

    const inline bool &Plane(size_t r, size_t c) const // Reference for getting or setting a value
    {
        return fields.Value(r, c);
    }

    friend ostream& operator<<(ostream& os, const Field& field);
};

struct Point
{
    size_t row;
    size_t col;
};

struct Support
{
    vector<Point> RowSupports; // Sets the row displacement at cornerPoint to zero
    vector<Point> ColSupports; // Sets the col displacement at cornerPoint to zero
};

struct Force
{
    Point attackCorner;
    float forceRow;
    float forceCol;
};


#endif
