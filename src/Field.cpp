#include "Field.hpp"

#define DIVSQRT2 0.70710678118

Field::Field(size_t rows, size_t cols) : fields(rows, cols, false), Rows(rows), Cols(cols)
{
}

ostream& operator<<(ostream& os, const Field& field) 
{
    return os << field.fields;
}
