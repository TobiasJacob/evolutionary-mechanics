/**
 * @file Field.cpp
 * @author Tobias Jacob - Galliera Raffaele - Ali Muddasar
 * @brief 
 * @version 1.0
 * @date 2020-12-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "Field.hpp"

#define DIVSQRT2 0.70710678118

Field::Field(size_t rows, size_t cols, bool defaultValue) : fields(rows, cols, defaultValue), Rows(rows), Cols(cols)
{
}

ostream& operator<<(ostream& os, const Field& field) 
{
    return os << field.fields;
}
