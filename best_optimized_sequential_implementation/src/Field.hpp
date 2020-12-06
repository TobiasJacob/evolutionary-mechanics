/**
 * @file Field.hpp
 * @author Tobias Jacob - Galliera Raffaele - Ali Muddasar
 * @brief 
 * @version 1.0
 * @date 2020-12-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef FIELD
#define FIELD

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include "Matrix.hpp"
#include "Equation.hpp"

using namespace std;

/**
 * @brief This class defines the abstraction of a Field, basically is composed by a boolean Matrix, where True values represent the presence of an element.
 * 
 */
class Field
{
private:
    Matrix<bool> fields; // Contains the bitvalues of which block is set

public:
    const size_t Rows, Cols; // Size of the field

    /**
     * @brief Construct a new Field object
     * 
     * @param rows 
     * @param cols 
     * @param defaultValue 
     */
    Field(size_t rows, size_t cols, bool defaultValue = false);

    /**
     * @brief Reference for getting or setting a value
     * 
     * @param r 
     * @param c 
     * @return true 
     * @return false 
     */
    inline bool &Plane(size_t r, size_t c)
    {
        return fields.Value(r, c);
    }

    /**
     * @brief Reference for getting or setting a value (const version)
     * 
     * @param r 
     * @param c 
     * @return true 
     * @return false 
     */
    const inline bool &Plane(size_t r, size_t c) const
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
