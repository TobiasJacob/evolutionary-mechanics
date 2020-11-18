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

    void Print();
    void SetTo(T value);
    vector<T> operator *(const vector<T> &vec);

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