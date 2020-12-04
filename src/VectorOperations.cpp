#include "VectorOperations.hpp"

template<typename T>
void add(const vector<T> &a, const vector<T> &b, vector<T> &result) 
{
    #ifdef DEBUG
    if (a.size() != b.size()) cerr << "Invalid vector addtion, first has " << a.size() << " rows and second has " << b.size() << " rows" << endl;
    if (a.size() != result.size()) cerr << "Invalid vector addtion, vectors have " << a.size() << " rows and result has " << result.size() << " rows" << endl;
    #endif
    #pragma omp for
    for (size_t r = 0; r < a.size(); r++)
        result[r] = a[r] + b[r];
}

template<typename T>
void subtract(const vector<T> &a, const vector<T> &b, vector<T> &result) 
{
    #ifdef DEBUG
    if (a.size() != b.size()) cerr << "Invalid vector subtract, first has " << a.size() << " rows and second has " << b.size() << " rows" << endl;
    if (a.size() != result.size()) cerr << "Invalid vector addtion, vectors have " << a.size() << " rows and result has " << result.size() << " rows" << endl;
    #endif
    #pragma omp for
    for (size_t r = 0; r < a.size(); r++)
        result[r] = a[r] - b[r];
}

template<typename T>
void multiply(const T a, const vector<T> &b, vector<T> &result) 
{
    #ifdef DEBUG
    if (b.size() != result.size()) cerr << "Invalid vector addtion, vector has " << b.size() << " rows and result has " << result.size() << " rows" << endl;
    #endif
    #pragma omp for
    for (size_t r = 0; r < b.size(); r++)
        result[r] = a * b[r];
}

template<typename T>
float l2square(const vector<T> &a) 
{
    float result = 0;
    // #pragma omp for reduction(+: result) // TODO
    for (size_t r = 0; r < a.size(); r++)
        result += a[r] * a[r];
    return result;    
}

template<typename T>
void printVector(const vector<T> &a) 
{
    #pragma omp for
    for (size_t r = 0; r < a.size(); r++)
        cout << a[r] << " ";
    cout << endl;
}

template<typename T>
void fillZeros(vector<T> &a) 
{
    #pragma omp for
    for (size_t r = 0; r < a.size(); r++)
        a[r] = 0;
}

template void add(const vector<int> &a, const vector<int> &b, vector<int> &result);
template void add(const vector<float> &a, const vector<float> &b, vector<float> &result);
template void subtract(const vector<int> &a, const vector<int> &b, vector<int> &result);
template void subtract(const vector<float> &a, const vector<float> &b, vector<float> &result);
template void multiply(const int a, const vector<int> &b, vector<int> &result);
template void multiply(const float a, const vector<float> &b, vector<float> &result);
template float l2square(const vector<int> &a);
template float l2square(const vector<float> &a);
template void printVector(const vector<int> &a);
template void printVector(const vector<float> &a);
template void fillZeros(vector<float> &a);
