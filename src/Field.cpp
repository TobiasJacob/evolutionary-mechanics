#include "Field.hpp"

#define DIVSQRT2 0.70710678118

Field::Field(int rows, int cols) : rows(rows), cols(cols), field(rows, cols, false), indexed(rows, cols, 0)
{
    field.value(1, 2) = true;
    field.value(2, 2) = true;
    field.value(2, 0) = true;
    field.value(1, 1) = true;
    field.value(1, 0) = true;
    field.value(2, 1) = true;
    // field.value(3, 1) = true;
    supports.push_back({.row = 2, .col = 0});
    supports.push_back({.row = 2, .col = 2});
    forces.push_back({.row = 1, .col = 1, .forceRow = 1, .forceCol = 0});
}

void Field::calculateIndex() 
{
    Matrix<int> indexed(rows, cols, 0);

    counter = 0;
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            if (field.value(r, c))
                indexed.value(r, c) = ++counter;

    indexed.print();
}

unique_ptr<Equation> Field::buildEquation() 
{
    calculateIndex();

    unique_ptr<Equation> result = make_unique<Equation>(2 * counter);

    float tau = 0.3, sigma = 1.0;

    Matrix<float> &C = result->C;
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            if (int cursorI = indexed.value(r, c))
            {
                if (r - 1 >= 0)
                    if (int &otherI = indexed.value(r - 1, c))
                    {
                        C.value(2 * cursorI - 2, 2 * cursorI - 2) -= sigma; // Pressure stress in rowIndex direction
                        C.value(2 * cursorI - 2, 2 * otherI - 2) += sigma;

                        C.value(2 * cursorI - 1, 2 * cursorI - 1) -= tau; // Shear stress in rowIndex direction
                        C.value(2 * cursorI - 1, 2 * otherI - 1) += tau;
                    }
                if (r + 1 < rows)
                    if (int &otherI = indexed.value(r + 1, c))
                    {
                        C.value(2 * cursorI - 2, 2 * cursorI - 2) -= sigma; // Pressure stress in colIndex direction
                        C.value(2 * cursorI - 2, 2 * otherI - 2) += sigma;

                        C.value(2 * cursorI - 1, 2 * cursorI - 1) -= tau; // Shear stress in rowIndex direction
                        C.value(2 * cursorI - 1, 2 * otherI - 1) += tau;
                    }

                if (c - 1 >= 0)
                    if (int &otherI = indexed.value(r, c - 1))
                    {
                        C.value(2 * cursorI - 1, 2 * cursorI - 1) -= sigma; // Pressure stress in colIndex direction
                        C.value(2 * cursorI - 1, 2 * otherI - 1) += sigma;

                        C.value(2 * cursorI - 2, 2 * cursorI - 2) -= tau; // Shear stress in rowIndex direction
                        C.value(2 * cursorI - 2, 2 * otherI - 2) += tau;
                    } 

                if (c + 1 < cols)
                    if (int &otherI = indexed.value(r, c + 1)) {
                        C.value(2 * cursorI - 1, 2 * cursorI - 1) -= sigma; // Pressure stress in colIndex direction
                        C.value(2 * cursorI - 1, 2 * otherI - 1) += sigma;

                        C.value(2 * cursorI - 2, 2 * cursorI - 2) -= tau; // Shear stress in rowIndex direction
                        C.value(2 * cursorI - 2, 2 * otherI - 2) += tau;
                    }

            }

    for (Support &support: supports)
    {
        int i = indexed.value(support.row, support.col);
        C.value(2 * i - 1, 2 * i - 1)--;
        C.value(2 * i - 2, 2 * i - 2)--;
    }

    vector<float> &f = result->f; // TODO Add gravity

    for (Force &force: forces)
    {
        int i = indexed.value(force.row, force.col);
        f[2 * i - 1] = force.forceCol;
        f[2 * i - 2] = force.forceRow;
    }

    return result;
}

double Field::calculatePerformance() 
{
    unique_ptr<Equation> equation = buildEquation();

    unique_ptr<vector<float>> epsilon = equation->solveIterative();

    return 0;
}


void Field::Print() 
{
    field.print();

}

