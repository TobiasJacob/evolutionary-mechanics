/**
 * @file Matrix.cpp
 * @author Tobias Jacob - Galliera Raffaele - Ali Muddasar
 * @brief 
 * @version 1.0
 * @date 2020-12-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "Matrix.hpp"

template<>
Matrix<int>::Matrix(size_t rows, size_t cols, int value) : rows(rows), cols(cols)
{
    values = (int *)malloc(sizeof(int) * rows * cols);
    memset(values, value, sizeof(int) * rows * cols);
}

template<>
Matrix<size_t>::Matrix(size_t rows, size_t cols, size_t value) : rows(rows), cols(cols)
{
    values = (size_t *)malloc(sizeof(size_t) * rows * cols);
    memset(values, value, sizeof(size_t) * rows * cols);
}

template<>
Matrix<bool>::Matrix(size_t rows, size_t cols, bool value) : rows(rows), cols(cols)
{
    values = (bool *)malloc(sizeof(bool) * rows * cols);
    memset(values, value, sizeof(bool) * rows * cols);
}

template<typename T>
Matrix<T>::Matrix(size_t rows, size_t cols, T value) : rows(rows), cols(cols)
{
    values = (T *)malloc(sizeof(T) * rows * cols);
    for (size_t r = 0; r < rows; r++)
        for (size_t c = 0; c < cols; c++)
            Value(r, c) = value;
}

template<typename T>
Matrix<T>::~Matrix() 
{
    if (values)
        free(values);
}

template<typename T>
size_t Matrix<T>::getRows() const
{
    return rows;
}

template<typename T>
size_t Matrix<T>::getCols() const
{
    return cols;
}

template<>
void Matrix<int>::SetTo(int value)
{
    memset(values, value, sizeof(float) * rows * cols);
}

template<typename T>
void Matrix<T>::SetTo(T value) 
{
    for (size_t r = 0; r < rows; r++)
        for (size_t c = 0; c < cols; c++)
            Value(r, c) = value;
    
}

template<>
void Matrix<bool>::Multiply(const vector<bool> &vec, vector<bool> &result) 
{
    #ifdef DEBUG
    if (vec.size() != cols) {cerr << "Invalid sparse matrix multiplication, has " << cols << " cols and vector has " << vec.size() << " rows" << endl; throw new invalid_argument("Vector size"); }
    if (rows != result.size()) {cerr << "Invalid sparse matrix multiplication, result has " << result.size() << " cols and matrix has " << rows << " rows" << endl; throw new invalid_argument("Result size"); }
    #endif
    for (size_t r = 0; r < rows; r++)
        for (size_t c = 0; c < cols; c++)
            if (Value(r, c) && vec[c])
                result[r] = !result[r];
}


template<typename T>
void Matrix<T>::Multiply(const vector<T> &vec, vector<T> &result) 
{
    #ifdef DEBUG
    if (vec.size() != cols) {cerr << "Invalid sparse matrix multiplication, has " << cols << " cols and vector has " << vec.size() << " rows" << endl; throw new invalid_argument("Vector size"); }
    if (rows != result.size()) {cerr << "Invalid sparse matrix multiplication, result has " << result.size() << " cols and matrix has " << rows << " rows" << endl; throw new invalid_argument("Result size"); }
    #endif
    for (size_t r = 0; r < rows; r++)
        for (size_t c = 0; c < cols; c++)
            result[r] += Value(r, c) * vec[c];
}

template class Matrix<int>;
template class Matrix<bool>;
template class Matrix<float>;
template class Matrix<size_t>;

template<typename T>
ostream& operator<<(ostream& os, const Matrix<T>& matrix) 
{
    for (size_t r = 0; r < matrix.rows; r++)
    {
        os << "[";
        for (size_t c = 0; c < matrix.cols; c++)
            os << std::setw(8) << setprecision(2) << matrix.Value(r, c) << ",";
        os << "],";
        os << endl;
    }
    return os;
}

template ostream& operator<<(ostream& os, const Matrix<int>& matrix);
template ostream& operator<<(ostream& os, const Matrix<bool>& matrix);
template ostream& operator<<(ostream& os, const Matrix<float>& matrix);
template ostream& operator<<(ostream& os, const Matrix<size_t>& matrix);
