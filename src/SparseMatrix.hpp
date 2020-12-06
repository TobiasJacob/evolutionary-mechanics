/**
 * @file SparseMatrix.hpp
 * @author Tobias Jacob (tj75@students.uwf.edu)
 * @brief Implements SparseMatrix
 * @version 1.0
 * @date 2020-12-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef SPARSEMATRIX
#define SPARSEMATRIX

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <list>

using namespace std;

// Forward declare for marking the << operator as friend
template<typename T> class SparseMatrix;
template <typename T> ostream& operator<<(ostream& os, const SparseMatrix<T>& matrix);

/**
 * @brief This class implements a sparse matrix including multiplication
 * 
 * @tparam T The datatypte to store the sparse matrix, ether int or float
 */
template<typename T>
class SparseMatrix
{
private:
    const size_t rows; // Rows of the sparse matrix
    const size_t cols; // Cols of the sparse matrix
    const T defaultValue; // The value to return if the matrix cell has no entry (typicall 0)
    vector<list<pair<size_t, T> > > values; // An vector of lists for each row

public:
    SparseMatrix(size_t rows, size_t cols);
    SparseMatrix(const SparseMatrix&) = delete; // Disallow copy
    SparseMatrix & operator=(const SparseMatrix&) = delete;
    SparseMatrix(SparseMatrix&&) = default; // Allow move
    SparseMatrix & operator=(SparseMatrix&&) = default;
    ~SparseMatrix();

    void SetValue(size_t row, size_t col, T value);
    const T& GetValue(size_t row, size_t col) const;
    T& GetOrAllocateValue(size_t row, size_t col);

    void Multiply(const vector<T> &vec, vector<T> &result);
    friend ostream& operator<< <T>(ostream& os, const SparseMatrix<T>& matrix);
};

#endif
