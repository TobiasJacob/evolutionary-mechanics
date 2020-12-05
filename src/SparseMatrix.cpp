#include "SparseMatrix.hpp"

template<typename T>
SparseMatrix<T>::SparseMatrix(size_t rows, size_t cols) : rows(rows), cols(cols), defaultValue(0)
{
    values = (list<pair<size_t, T>> *)malloc(rows * sizeof(list<pair<size_t, T>>));
    new (values) list<pair<size_t, T>>[rows]; // Use replacement new to construct vectors in already allocated (and later also aligned) memory
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

    list<pair<size_t, T>> &rowVals = values[row];

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
        rowVals.insert(cursorIterator, pair<size_t, T>(col, value)); // Insert new value before cursorIterator
}

template<typename T>
const T& SparseMatrix<T>::GetValue(size_t row, size_t col) const
{
    #if DEBUG
    if (row >= rows || col >= cols) throw new out_of_range("SparseMatrix");
    #endif

    list<pair<size_t, T>> &rowVals = values[row];

    auto cursorIterator = rowVals.begin();
    while (cursorIterator != rowVals.end())
    {
        pair<size_t, T> &cursor = *cursorIterator;
        if (cursor.first == col) return cursor.second;
        cursorIterator++;
    }

    return defaultValue;
}

template<typename T>
T& SparseMatrix<T>::GetOrAllocateValue(size_t row, size_t col) 
{
    #if DEBUG
    if (row >= rows || col >= cols) throw new out_of_range("SparseMatrix");
    #endif

    list<pair<size_t, T>> &rowVals = values[row];

    // Find end or first value bigger than cursor
    auto cursorIterator = rowVals.begin();
    while (cursorIterator != rowVals.end())
    {
        if (cursorIterator->first >= col) break;
        cursorIterator++;
    }

    if (cursorIterator != rowVals.end() && cursorIterator->first == col)
        return cursorIterator->second; // Get this value
    else
    {
        rowVals.insert(cursorIterator, pair<size_t, T>(col, 0)); // Insert new value before cursorIterator
        return (--cursorIterator)->second;
    }
}

template<typename T>
vector<T> SparseMatrix<T>::operator *(const vector<T> &vec) 
{
    #ifdef DEBUG
    if (vec.size() != cols) throw invalid_argument("Vec has wrong size");
    #endif
    vector<T> result(rows, 0);
    for (size_t r = 0; r < rows; r++)
        for (pair<size_t, T> &element: values[r])
        {
            result[r] += element.second * vec[element.first];
        }
    return result;
}

template<typename T>
ostream& operator<<(ostream& os, const SparseMatrix<T>& matrix)
{
    for (size_t r = 0; r < matrix.rows; r++)
    {
        os << "[";
        for (size_t c = 0; c < matrix.cols; c++)
            os << std::setw(8) << setprecision(2) << matrix.GetValue(r, c) << ","; // Not the most efficent way to do it, but we are not going to print big matricies. 
        os << "],";
        os << endl;
    }
    return os;
}

template class SparseMatrix<int>;
template class SparseMatrix<float>;
template ostream& operator<<(ostream& os, const SparseMatrix<int>& matrix);
template ostream& operator<<(ostream& os, const SparseMatrix<float>& matrix);