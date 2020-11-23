#ifndef MATRIX
#define MATRIX

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <list>

using namespace std;

template<typename T>
class SparseMatrix
{
private:
    const int rows;
    const int cols;
    T defaultValue;
    list<pair<int, T>> *values;

public:
    SparseMatrix(int rows, int cols, T defaultValue);
    SparseMatrix(const SparseMatrix&) = delete; // Disallow copy
    SparseMatrix & operator=(const SparseMatrix&) = delete;
    SparseMatrix(SparseMatrix&&) = default; // Allow move
    SparseMatrix & operator=(SparseMatrix&&) = default;
    ~SparseMatrix();

    void SetValue(int row, int col, T value);
    const T& GetValue(int row, int col);

    vector<T> operator *(const vector<T> &vec);

    // This one should be declared inline because matrix is a template class.
    friend ostream& operator<<(ostream& os, const SparseMatrix<T>& matrix) 
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

#endif
