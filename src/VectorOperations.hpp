/**
 * @file VectorOperations.hpp
 * @author Tobias Jacob - Galliera Raffaele - Alì Muddasar
 * @brief this code contains all the functions that operate on the Vector. 
 * @version 1.0
 * @date 2020-12-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

/**
 * @brief gets two vectors and calculates their sum in a third vector
 * 
 * @tparam T 
 * @param a 
 * @param b 
 * @param result 
 */
template<typename T>
void add(const vector<T> &a, const vector<T> &b, vector<T> &result);

/**
 * @brief gets two vectors and calculates their difference in a third vector 
 * 
 * @tparam T 
 * @param a 
 * @param b 
 * @param result 
 */
template<typename T>
void subtract(const vector<T> &a, const vector<T> &b, vector<T> &result);

/**
 * @brief gets two vectors and calculates their multiplication in a third vector
 * 
 * @tparam T 
 * @param a 
 * @param b 
 * @param result 
 */
template<typename T>
void multiply(const T a, const vector<T> &b, vector<T> &result);

/**
 * @brief gets a vector and calculates its the L2 value squared
 * 
 * @tparam T 
 * @param a 
 * @param resultL2Square 
 */
template<typename T>
void l2square(const vector<T> &a, T &resultL2Square); 

/**
 * @brief gets two vector and calculates the value of their scalar product
 * 
 * @tparam T 
 * @param a 
 * @param b 
 * @param resultScalar 
 */
template<typename T>
void scalarProduct(const vector<T> &a, const vector<T> &b, T &resultScalar);

/**
 * @brief gets a vector and prints all its values
 * 
 * @tparam T 
 * @param a 
 */
template<typename T>
void printVector(const vector<T> &a); 

/**
 * @brief gets a vector and initialize it with 0 
 * 
 * @tparam T 
 * @param a 
 */
template<typename T>
void fillZeros(vector<T> &a); 

/**
 * @brief assign the values of one source vector to a destination vector
 * 
 * @tparam T 
 * @param src 
 * @param dest 
 */
template<typename T>
void assign(vector<T> &src, vector<T> &dest); 
