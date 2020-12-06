/**
 * @file Matrix.hpp
 * @author Tobias Jacob - Galliera Raffaele - Ali Muddasar
 * @brief 
 * @version 1.0
 * @date 2020-12-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef MATRIX
#define MATRIX

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

// Forward declare for marking the << operator as friend
template<typename T> class Matrix;
template <typename T> ostream& operator<<(ostream& os, const Matrix<T>& matrix);
template<typename T>

/**
 * @brief Defines Matrix behaviour used by the construction of our Fields
 * 
 */
class Matrix
{
private:
    const size_t rows;
    const size_t cols;
    T *values;

public:
    Matrix(size_t rows, size_t cols, T value);
    Matrix(const Matrix&) = delete; // Disallow copy
    Matrix & operator=(const Matrix&) = delete;
    Matrix(Matrix&&) = default; // Allow move
    Matrix & operator=(Matrix&&) = default;
    ~Matrix();

    size_t getRows() const;
    size_t getCols() const;

    /**
     * @brief Returns the value given row and column
     * 
     * @param r row
     * @param c column
     * @return T& 
     */
    inline T &Value(size_t r, size_t c) 
    {
        #ifdef DEBUG
        if (r >= rows || c >= cols) cerr << "Index access error (" << r << "," << c << ") out of (" << rows << "," << cols << ")" << endl;
        #endif
        return values[r * cols + c];
    }

    /**
     * @brief Returns the value given row and column
     * If this class is marked as const, this const overload method will return a const (read only) reference
     * 
     * @param r 
     * @param c 
     * @return const T& 
     */
    inline const T &Value(size_t r, size_t c) const
    {
        #ifdef DEBUG
        if (r >= rows || c >= cols) cerr << "Index access error (" << r << "," << c << ") out of (" << rows << "," << cols << ")" << endl;
        #endif
        return values[r * cols + c];
    }

    void SetTo(T value);
    void Multiply(const vector<T> &vec, vector<T> &result);

    friend ostream& operator<<<T>(ostream& os, const Matrix<T>& matrix);
};

#endif
