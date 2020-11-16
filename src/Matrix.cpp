#include "Matrix.hpp"

template<>
Matrix<int>::Matrix(int rows, int cols, int value) : rows(rows), cols(cols)
{
    values = (int *)malloc(sizeof(float) * rows * cols);
    memset(values, value, sizeof(float) * rows * cols);
}

template<>
Matrix<bool>::Matrix(int rows, int cols, bool value) : rows(rows), cols(cols)
{
    values = (bool *)malloc(sizeof(float) * rows * cols);
    memset(values, value, sizeof(float) * rows * cols);
}

template<typename T>
Matrix<T>::Matrix(int rows, int cols, T value) : rows(rows), cols(cols)
{
    values = (T *)malloc(sizeof(float) * rows * cols);
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            Value(r, c) = value;
}

template<typename T>
Matrix<T>::~Matrix() 
{
    if (values)
        free(values);
}

template<typename T>
void Matrix<T>::Print() 
{
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
            cout << std::setw(8) << setprecision(2) << Value(r, c);
        cout << endl;
    }
}

template<>
void Matrix<int>::SetTo(int value)
{
    memset(values, value, sizeof(float) * rows * cols);
}

template<typename T>
void Matrix<T>::SetTo(T value) 
{
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            Value(r, c) = value;
    
}

template class Matrix<int>;
template class Matrix<bool>;
template class Matrix<float>;
