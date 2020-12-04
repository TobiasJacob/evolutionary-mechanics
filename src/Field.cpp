#include "Field.hpp"

#define DIVSQRT2 0.70710678118

Field::Field(int rows, int cols) : fields(rows, cols, false), Rows(rows), Cols(cols)
{
}

ostream& operator<<(ostream& os, const Field& field) 
{
    return os << field.fields;
}
