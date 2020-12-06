#include "VectorOperations.hpp"

template<typename T>
void add(const vector<T> &a, const vector<T> &b, vector<T> &result) 
{
    #ifdef DEBUG
    if (a.size() != b.size()) cerr << "Invalid vector addtion, first has " << a.size() << " rows and second has " << b.size() << " rows" << endl;
    if (a.size() != result.size()) cerr << "Invalid vector addtion, vectors have " << a.size() << " rows and result has " << result.size() << " rows" << endl;
    #endif
    for (size_t r = 0; r < a.size(); r++)
        result[r] = a[r] + b[r]; //the result vector is filled with the sum of the values of vectors a and b 
}

template<typename T>
void subtract(const vector<T> &a, const vector<T> &b, vector<T> &result) 
{
    #ifdef DEBUG
    if (a.size() != b.size()) cerr << "Invalid vector subtract, first has " << a.size() << " rows and second has " << b.size() << " rows" << endl;
    if (a.size() != result.size()) cerr << "Invalid vector addtion, vectors have " << a.size() << " rows and result has " << result.size() << " rows" << endl;
    #endif
    for (size_t r = 0; r < a.size(); r++)
        result[r] = a[r] - b[r]; //the result vector is filled with the difference of the values of vectors a and b 
}

template<typename T>
void multiply(const T a, const vector<T> &b, vector<T> &result) 
{
    #ifdef DEBUG
    if (b.size() != result.size()) cerr << "Invalid vector addtion, vector has " << b.size() << " rows and result has " << result.size() << " rows" << endl;
    #endif
    for (size_t r = 0; r < b.size(); r++)
        result[r] = a * b[r]; //the result vector is filled with the multiplication of the values of vectors a and b 
}

template<typename T>
void l2square(const vector<T> &a, T &resultL2Square)
{
    for (size_t r = 0; r < a.size(); r++)
        resultL2Square += a[r] * a[r]; //the summation of the square values of the vector a is saved in resultL2Square
}

template<typename T>
void scalarProduct(const vector<T> &a, const vector<T> &b, T &resultScalar) 
{
    #ifdef DEBUG
    if (a.size() != b.size()) cerr << "Invalid vector addtion, a has " << a.size() << " rows and b has " << b.size() << " rows" << endl;
    #endif
    for (size_t r = 0; r < a.size(); r++)
        resultScalar += a[r] * b[r]; //the summation of the multiplication of values of vector a and b is saved in resultScalar
}

template<typename T>
void printVector(const vector<T> &a) 
{
    for (size_t r = 0; r < a.size(); r++)
        cout << a[r] << " "; //all the values of the vector a are printed
    cout << endl;
}

template<typename T>
void fillZeros(vector<T> &a) 
{
    for (size_t r = 0; r < a.size(); r++)
        a[r] = 0; //the vector a is filled with 0 
}

template<typename T>
void assign(vector<T> &src, vector<T> &dest) 
{
    #ifdef DEBUG
    if (src.size() != dest.size()) cerr << "Invalid vector assignment, a has " << src.size() << " rows and b has " << dest.size() << " rows" << endl;
    #endif
    for (size_t r = 0; r < src.size(); r++)
        dest[r] = src[r]; //all the values of vector src are copied in the vector dest
}

template void add(const vector<int> &a, const vector<int> &b, vector<int> &result);
template void add(const vector<float> &a, const vector<float> &b, vector<float> &result);
template void subtract(const vector<int> &a, const vector<int> &b, vector<int> &result);
template void subtract(const vector<float> &a, const vector<float> &b, vector<float> &result);
template void multiply(const int a, const vector<int> &b, vector<int> &result);
template void multiply(const float a, const vector<float> &b, vector<float> &result);
template void l2square(const vector<int> &a, int &resultL2Square);
template void l2square(const vector<float> &a, float &resultL2Square);
template void scalarProduct(const vector<int> &a, const vector<int> &b, int &result);
template void scalarProduct(const vector<float> &a, const vector<float> &b, float &result);
template void printVector(const vector<int> &a);
template void printVector(const vector<float> &a);
template void fillZeros(vector<float> &a);
template void assign(vector<float> &src, vector<float> &dest);
