#include "Field.hpp"

#define DIVSQRT2 0.70710678118

Field::Field(int rows, int cols) : Rows(rows), Cols(cols), field(rows, cols, false), indexed(rows, cols, 0)
{
}

void Field::calculateIndex() 
{
    Matrix<int> indexed(Rows, Cols, 0);

    counter = 0;
    for (int r = 0; r < Rows; r++)
        for (int c = 0; c < Cols; c++)
            if (field.value(r, c))
                indexed.value(r, c) = ++counter;

    indexed.print();
}

void Field::Print() 
{
    field.print();

}

