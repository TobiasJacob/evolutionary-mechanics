#include "Field.hpp"

#define DIVSQRT2 0.70710678118

Field::Field(int rows, int cols) : fields(rows, cols, false), cornerIndex(rows + 1, cols + 1, 0), Rows(rows), Cols(cols)
{
}

void Field::CalculateIndex() 
{
    cornerIndex.SetTo(0);

    counter = 0;
    for (int r = 0; r < Rows; r++)
        for (int c = 0; c < Cols; c++)
            if (fields.Value(r, c))
            {
                if (!cornerIndex.Value(r, c)) cornerIndex.Value(r, c) = ++counter;
                if (!cornerIndex.Value(r + 1, c)) cornerIndex.Value(r + 1, c) = ++counter;
                if (!cornerIndex.Value(r + 1, c + 1)) cornerIndex.Value(r + 1, c + 1) = ++counter;
                if (!cornerIndex.Value(r, c + 1)) cornerIndex.Value(r, c + 1) = ++counter;
            }
}

void Field::Print() 
{
    fields.Print();

}

int Field::GetCounter() 
{
    return counter;
}

