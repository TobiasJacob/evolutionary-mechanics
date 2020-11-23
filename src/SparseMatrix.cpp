#include "SparseMatrix.hpp"

template<typename T>
SparseMatrix<T>::SparseMatrix(int rows, int cols, T defaultValue) : rows(rows), cols(cols), defaultValue(defaultValue)
{
    values = (list<pair<int, T>> *)malloc(rows * sizeof(list<pair<int, T>>));
    new (values) list<pair<int, T>>[rows]; // Use replacement new to construct vectors in already allocated (and later also aligned) memory
}

template<typename T>
SparseMatrix<T>::~SparseMatrix() 
{
    free(values);
}

template<typename T>
void SparseMatrix<T>::SetValue(size_t row, size_t col, T value) 
{
    #if DEBUG
    if (row >= rows || col >= cols) throw new out_of_range("SparseMatrix");
    #endif

    list<pair<int, T>> &rowVals = values[row];

    // Find end or first value bigger than cursor
    auto cursorIterator = rowVals.begin();
    while (cursorIterator != rowVals.end())
    {
        if (cursorIterator->first >= col) break;
        cursorIterator++;
    }

    if (cursorIterator != rowVals.end() && cursorIterator->first == col)
        cursorIterator->second = value; // Set this value
    else
        rowVals.insert(cursorIterator, pair<int, T>(col, value)); // Insert new value before cursorIterator
}

template<typename T>
const T& SparseMatrix<T>::GetValue(size_t row, size_t col) 
{
    #if DEBUG
    if (row >= rows || col >= cols) throw new out_of_range("SparseMatrix");
    #endif

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

template class SparseMatrix<int>;
template class SparseMatrix<float>;