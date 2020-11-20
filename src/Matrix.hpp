#ifndef MATRIX
#define MATRIX

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

template<typename T>
class Matrix
{
private:
    const int rows;
    const int cols;
    T *values;

public:
    Matrix(int rows, int cols, T value);
    Matrix(const Matrix&) = delete; // Disallow copy
    Matrix & operator=(const Matrix&) = delete;
    Matrix(Matrix&&) = default; // Allow move
    Matrix & operator=(Matrix&&) = default;
    ~Matrix();

    inline T &Value(int r, int c) 
    {
        #ifdef DEBUG
        if (r >= rows || c >= cols) cerr << "Index access error (" << r << "," << c << ") out of (" << rows << "," << cols << ")" << endl;
        #endif
        return values[r * cols + c];
    }

    // If this class is marked as const, this const overload method will return a const (read only) reference
    inline const T &Value(int r, int c) const
    {
        #ifdef DEBUG
        if (r >= rows || c >= cols) cerr << "Index access error (" << r << "," << c << ") out of (" << rows << "," << cols << ")" << endl;
        #endif
        return values[r * cols + c];
    }

    void SetTo(T value);
    vector<T> operator *(const vector<T> &vec);

    // This one should be declared inline because matrix is a template class.
    friend ostream& operator<<(ostream& os, const Matrix<T>& matrix) 
    {
        for (int r = 0; r < matrix.rows; r++)
        {
            os << "[";
            for (int c = 0; c < matrix.cols; c++)
                os << std::setw(8) << setprecision(2) << matrix.Value(r, c) << ",";
            os << "],";
            os << endl;
        }
        return os;
    }
};

template<typename T>
vector<T> add(const vector<T> &a, const vector<T> &b);

template<typename T>
vector<T> subtract(const vector<T> &a, const vector<T> &b);

template<typename T>
vector<T> multiply(const T a, const vector<T> &b);

template<typename T>
float l2square(const vector<T> &a);

template<typename T>
void printVector(const vector<T> &a);

#endif
