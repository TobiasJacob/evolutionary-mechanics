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

template<typename T>
class SparseMatrix
{
private:
    const size_t rows;
    const size_t cols;
    const T defaultValue;
    vector<list<pair<size_t, T> > > values;

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
