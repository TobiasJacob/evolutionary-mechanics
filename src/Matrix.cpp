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
        cout << "[";
        for (int c = 0; c < cols; c++)
            cout << std::setw(8) << setprecision(2) << Value(r, c) << ",";
        cout << "],";
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

template<>
vector<bool> Matrix<bool>::operator *(const vector<bool> &vec) 
{
    #ifdef DEBUG
    if (vec.size() != cols) cerr << "Invalid matrix multiplication, has " << cols << " cols and vector has " << vec.size() << " rows" << endl;
    #endif
    vector<bool> result(rows, 0);
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            if (Value(r, c) * vec[c])
                result[r] = !result[r];
    return result;    
}


template<typename T>
vector<T> Matrix<T>::operator *(const vector<T> &vec) 
{
    #ifdef DEBUG
    if (vec.size() != cols) cerr << "Invalid matrix multiplication, has " << cols << " cols and vector has " << vec.size() << " rows" << endl;
    #endif
    vector<T> result(rows, 0);
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            result[r] += Value(r, c) * vec[c];
    return result;    
}

template class Matrix<int>;
template class Matrix<bool>;
template class Matrix<float>;

template<typename T>
vector<T> add(const vector<T> &a, const vector<T> &b) 
{
    #ifdef DEBUG
    if (a.size() != b.size()) cerr << "Invalid vector addtion, first has " << a.size() << " rows and second has " << b.size() << " rows" << endl;
    #endif
    vector<T> result(a.size(), 0);
    for (int r = 0; r < a.size(); r++)
        result[r] = a[r] + b[r];
    return result;    
}

template<typename T>
vector<T> subtract(const vector<T> &a, const vector<T> &b) 
{
    #ifdef DEBUG
    if (a.size() != b.size()) cerr << "Invalid vector subtract, first has " << a.size() << " rows and second has " << b.size() << " rows" << endl;
    #endif
    vector<T> result(a.size(), 0);
    for (int r = 0; r < a.size(); r++)
        result[r] = a[r] - b[r];
    return result;    
}

template<typename T>
vector<T> multiply(const T a, const vector<T> &b) 
{
    vector<T> result(b.size(), 0);
    for (int r = 0; r < b.size(); r++)
        result[r] = a * b[r];
    return result;    
}

template<typename T>
float l2square(const vector<T> &a) 
{
    float result = 0;
    for (int r = 0; r < a.size(); r++)
        result += a[r] * a[r];
    return result;    
}

template<typename T>
void printVector(const vector<T> &a) 
{
    for (int r = 0; r < a.size(); r++)
        cout << a[r] << " ";
    cout << endl;
}

template vector<int> add(const vector<int> &a, const vector<int> &b);
template vector<float> add(const vector<float> &a, const vector<float> &b);
template vector<int> subtract(const vector<int> &a, const vector<int> &b);
template vector<float> subtract(const vector<float> &a, const vector<float> &b);
template vector<int> multiply(const int a, const vector<int> &b);
template vector<float> multiply(const float a, const vector<float> &b);
template float l2square(const vector<int> &a);
template float l2square(const vector<float> &a);
template void printVector(const vector<int> &a);
template void printVector(const vector<float> &a);
