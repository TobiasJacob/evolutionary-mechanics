/**
 * @file Equation.hpp
 * @author Tobias Jacob (tj75@students.uwf.edu)
 * @brief Implements Equation
 * @version 1.0
 * @date 2020-12-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef EQUATION
#define EQUATION

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <math.h>
#include <iomanip>
#include "Matrix.hpp"
#include "Microtime.hpp"

using namespace std;

/**
 * @brief This class is able to solve a symmetric, postive definite and sparse matrix equation system (K * u = f) using the conjugate gradient (CG) method.
 * 
 */
struct Equation
{
private:
    // Temporary values used in the CG method. Have to be allocated globally to be accessable from multiple threads.
    // x_k, r_k and p_k have to be swappable, therefore a pointer is needed
    unique_ptr<vector<float>> x_k;
    unique_ptr<vector<float>> r_k;
    unique_ptr<vector<float>> p_k;

    unique_ptr<vector<float>> x_k1;
    unique_ptr<vector<float>> r_k1;
    unique_ptr<vector<float>> p_k1;

    // Temporary values for the CG method, but they don't have to be swapped.
    vector<float> kTimesP;
    vector<float> kTimesx_k;
    vector<float> scaledP_K;
    vector<float> scaledKTimesP;
    float r_k1_squared = 0;
    float alpha_k_divider = 0;
    float alpha_k = 0;

    // Values accessed by the primary thread
    size_t counter = 0; // Steps performed
    const size_t maxSteps = 10000; // Maximum number of steps
public:
    const size_t N; // Size of the equation system.

    Matrix<float> K; // The stiffness matrix
    vector<float> f; // The forces on the mechanical structure

    /**
     * @brief Construct a new Equation object
     * 
     * @param N The equation size
     */
    Equation(const size_t N);

    /**
     * @brief Calculates the solution u in parallel
     * 
     */
    void SolveIterative();

    /**
     * @brief Get the solution vector u
     * 
     * @return vector<float>& u, the solution
     */
    vector<float> &GetSolution();

    /**
     * @brief How many steps were needed to solve the equation
     * 
     * @return int steps needed to solve the equation
     */
    int GetSteps();

    /**
     * @brief Prints the equation
     * 
     * @param os The output stream
     * @param matrix The equation to print
     * @return ostream& The output stream
     */
    friend ostream& operator<<(ostream& os, const Equation& equation);
};

#endif
