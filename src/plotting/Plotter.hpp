/**
 * @file Plotter.hpp
 * @author Tobias Jacob - Raffaele Galliera - Ali Muddasar
 * @brief 
 * @version 1.0
 * @date 2020-12-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef PLOTTER
#define PLOTTER

#include <iostream>
#include <fstream>
#include <string>

#include "../Equation.hpp"
#include "../Field.hpp"

using namespace std;

/**
 * @brief This class takes care of updating the "Plotter.html" (by default) file located in the /src/plotting, defining Javascript variables of the given structure
 * 
 */
class Plotter
{
private:
    ofstream output;// Output file
    string head;// Placeholder string of the starting position for defining the values
    string tail;// Placeholder string of the final position for defining the values
public:
    /**
     * @brief Construct a new Plotter object
     * 
     * @param fileName Output file to be set
     */
    Plotter(string fileName);
    /**
     * @brief Plots the fields informations in Javascript valid syntax
     * 
     * @param field 
     * @param u 
     * @param cornerIndexRow 
     * @param cornerIndexCol 
     * @param supports 
     * @param forces 
     * @param solutionSteps 
     * @param residum 
     * @param stress 
     * @param time 
     */
    void plot(const Field &field, const vector<float> &u, const Matrix<size_t> &cornerIndexRow, const Matrix<size_t> &cornerIndexCol, const Support &supports, const vector<Force> &forces, const size_t solutionSteps, const float residum, const vector<float> &stress, float time);
    ~Plotter();
};

#endif