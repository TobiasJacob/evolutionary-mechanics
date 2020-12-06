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
    const T defaultValue; // The value to return if the matrix cell has no entry, is zero.
    vector<list<pair<size_t, T> > > values; // An vector of lists for each row

public:
    SparseMatrix(const SparseMatrix&) = delete; // Disallow copy
    SparseMatrix & operator=(const SparseMatrix&) = delete;
    SparseMatrix(SparseMatrix&&) = default; // Allow move
    SparseMatrix & operator=(SparseMatrix&&) = default;

    /**
     * @brief Construct a new Sparse Matrix object
     * 
     * @param rows Rows of the matrix
     * @param cols Cols of the matrix
     */
    SparseMatrix(size_t rows, size_t cols);

    /**
     * @brief Destroy the Sparse Matrix object
     * 
     */
    ~SparseMatrix();

    /**
     * @brief Sets the value of a cell. If the cell exists, it is overwritten. If it does not exist, it is created.
     * 
     * @param row The row of the cell
     * @param col The col of the cell
     * @param value The value to set
     */
    void SetValue(size_t row, size_t col, T value);

    /**
     * @brief Get the (read-only) value of a cell. If cell does not exist, it returns zero and does not allocate a new cell.
     * 
     * @param row Row of the cell
     * @param col Col of the cell
     * @return const T& A const reference to the cell
     */
    const T& GetValue(size_t row, size_t col) const;

    /**
     * @brief Returns a refrence to a matrix cell. If cell does not exist, it is allocated.
     * 
     * @param row Row of the cell
     * @param col Col of the cell
     * @return T& A reference to the cell
     */
    T& GetOrAllocateValue(size_t row, size_t col);

    /**
     * @brief Performs a parallelized sparse matrix multiplication with a vectore
     * 
     * @param vec The vector. Size must be rows of this matrix.
     * @param result The output vector to write to. Size must be cols of this matrix.
     */
    void Multiply(const vector<T> &vec, vector<T> &result);

    /**
     * @brief Prints the sparse matrix
     * 
     * @param os The output stream
     * @param matrix The matrix to print
     * @return ostream& The output stream
     */
    friend ostream& operator<< <T>(ostream& os, const SparseMatrix<T>& matrix);
};

#endif
