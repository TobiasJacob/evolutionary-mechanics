#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

template<typename T>
void add(const vector<T> &a, const vector<T> &b, vector<T> &result);

template<typename T>
void subtract(const vector<T> &a, const vector<T> &b, vector<T> &result);

template<typename T>
void multiply(const T a, const vector<T> &b, vector<T> &result);

template<typename T>
T l2square(const vector<T> &a);

template<typename T>
void scalarProduct(const vector<T> &a, const vector<T> &b, T &resultScalar);

template<typename T>
void printVector(const vector<T> &a);

template<typename T>
void fillZeros(vector<T> &a);
