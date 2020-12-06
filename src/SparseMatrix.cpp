#include "SparseMatrix.hpp"

template<typename T>
SparseMatrix<T>::SparseMatrix(size_t rows, size_t cols) : rows(rows), cols(cols), defaultValue(0), values(rows)
{
}

template<typename T>
SparseMatrix<T>::~SparseMatrix() 
{
}

template<typename T>
void SparseMatrix<T>::SetValue(size_t row, size_t col, T value) 
{
    #if DEBUG
    if (row >= rows || col >= cols) throw new out_of_range("SparseMatrix");
    #endif

    // Get row
    list<pair<size_t, T>> &rowVals = values[row];

    // Find end or first value greater or equal to cursor
    auto cursorIterator = rowVals.begin();
    while (cursorIterator != rowVals.end())
    {
        // If index of cell is greater than col break
        if (cursorIterator->first >= col) break;
        cursorIterator++;
    }
    
    // Check if cell with that col index is found
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

    // Get row
    const list<pair<size_t, T>> &rowVals = values[row];

    // Iterate through the row
    auto cursorIterator = rowVals.begin();
    while (cursorIterator != rowVals.end())
    {
        const pair<size_t, T> &cursor = *cursorIterator;
        // If curser is the current value, return it
        if (cursor.first == col) return cursor.second;
        // If index of cell is greater than col break
        if (cursorIterator->first > col) break;
        cursorIterator++;
    }

    // If nothing is found, return the default value
    return defaultValue;
}

template<typename T>
T& SparseMatrix<T>::GetOrAllocateValue(size_t row, size_t col) 
{
    #if DEBUG
    if (row >= rows || col >= cols) throw new out_of_range("SparseMatrix");
    #endif

    // Get row
    list<pair<size_t, T>> &rowVals = values[row];

    // Find end or first value greater or equal to cursor
    auto cursorIterator = rowVals.begin();
    while (cursorIterator != rowVals.end())
    {
        if (cursorIterator->first >= col) break;
        cursorIterator++;
    }

    // Check if the actual value is found
    if (cursorIterator != rowVals.end() && cursorIterator->first == col)
        return cursorIterator->second; // Return the value
    else
    {
        rowVals.insert(cursorIterator, pair<size_t, T>(col, 0)); // Insert new value before cursorIterator
        return (--cursorIterator)->second; // Return the new value
    }
}

template<typename T>
void SparseMatrix<T>::Multiply(const vector<T> &vec, vector<T> &result) 
{
    #ifdef DEBUG
    if (vec.size() != cols) {cerr << "Invalid sparse matrix multiplication, has " << cols << " cols and vector has " << vec.size() << " rows" << endl; throw new invalid_argument("Vector size"); }
    if (rows != result.size()) {cerr << "Invalid sparse matrix multiplication, result has " << result.size() << " cols and matrix has " << rows << " rows" << endl; throw new invalid_argument("Result size"); }
    #endif
    // Each thread processes its own chunk rows
    #pragma omp for schedule(static, 32)
    for (size_t r = 0; r < rows; r++)
        for (pair<size_t, T> &element: values[r]) // Iterate over all elements in that row
        {
            // Perform the matrix multiplication (element.first is cell index, element.second is cell value)
            result[r] += element.second * vec[element.first];
        }
}

template<typename T>
ostream& operator<<(ostream& os, const SparseMatrix<T>& matrix)
{
    // Print each row
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