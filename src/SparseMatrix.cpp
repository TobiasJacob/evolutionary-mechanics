#include "SparseMatrix.hpp"

template<typename T>
SparseMatrix<T>::SparseMatrix(int rows, int cols, T defaultValue) : rows(rows), cols(cols), defaultValue(defaultValue)
{
    values = malloc(rows * sizeof(list<pair<int, T>>));
    new (values) list<pair<int, T>>[rows]; // Use replacement new to construct vectors in already allocated (and later also aligned) memory
}

template<typename T>
SparseMatrix<T>::~SparseMatrix() 
{
    free(values);
}

template<typename T>
void SparseMatrix<T>::SetValue(int row, int col, T value) 
{
    list<pair<int, T>> &rowVals = values[row];

    // Find end or first value bigger than cursor
    auto cursorIterator = rowVals.begin();
    while (cursorIterator != rowVals.end())
    {
        pair<int, T> &cursor = *cursorIterator;
        if (cursor.first >= col) break;
        cursorIterator++;
    }

    
    pair<int, T> &cursor = *cursorIterator;
    if (cursor.first == col)
        cursor.second = value; // Set this value
    else
        rowVals.insert(cursorIterator, value); // Insert new value before cursorIterator
}

template<typename T>
const T& SparseMatrix<T>::GetValue(int row, int col) 
{
    list<pair<int, T>> &rowVals = values[row];

    auto cursorIterator = rowVals.begin();
    while (cursorIterator != rowVals.end())
    {
        pair<int, T> &cursor = *cursorIterator;
        if (cursor.first == col) return cursor.second;
        cursorIterator++;
    }

    return defaultValue;    
}
