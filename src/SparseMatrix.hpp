#ifndef SPARSEMATRIX
#define SPARSEMATRIX

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
    const T defaultValue;
    list<pair<int, T>> *values;

public:
    SparseMatrix(int rows, int cols);
    SparseMatrix(const SparseMatrix&) = delete; // Disallow copy
    SparseMatrix & operator=(const SparseMatrix&) = delete;
    SparseMatrix(SparseMatrix&&) = default; // Allow move
    SparseMatrix & operator=(SparseMatrix&&) = default;
    ~SparseMatrix();

    void SetValue(size_t row, size_t col, T value);
    const T& GetValue(size_t row, size_t col);

    vector<T> operator *(const vector<T> &vec);
};

#endif
