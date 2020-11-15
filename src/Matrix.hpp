#ifndef MATRIX
#define MATRIX

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <iomanip>

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
    Matrix(const Matrix&) = delete;
    Matrix & operator=(const Matrix&) = delete;
    ~Matrix();

    inline T &Value(int r, int c) 
    {
        #ifdef DEBUG
        if (r >= rows || c >= cols) cerr << "Index access error (" << r << "," << c << ") out of (" << rows << "," << cols << ")" << endl;
        #endif
        return values[r * cols + c];
    }

    void Print();
};

#endif