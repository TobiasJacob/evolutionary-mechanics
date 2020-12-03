#include "Plotter.hpp"

Plotter::Plotter(string fileName): output(fileName)
{
}

// Requires not empty fields, etc.
void Plotter::plot(Field &field, vector<float> &u, Matrix<int> &cornerIndexRow, Matrix<int> &cornerIndexCol) 
{
    // field
    output << "field = [";
    output << "[" << field.Plane(0, 0);
    for (int c = 1; c < field.Cols; c++)
        output << ", " << field.Plane(0, c);
    output << "]";
    for (int r = 1; r < field.Rows; r++)
    {
        output << ", [" << field.Plane(r, 0);
        for (int c = 1; c < field.Cols; c++)
            output << ", " << field.Plane(r, c);
        output << "]";
    }
    output << "];" << endl;

    // u
    output << "u = [" << u[0];
    for (int i = 1; i < field.Cols; i++)
        output << ", " << u[i];
    output << "];" << endl;

    
    // cornerIndexRow
    output << "cornerIndexRow = [";
    output << "[" << cornerIndexRow.Value(0, 0);
    for (int c = 1; c < cornerIndexRow.getCols(); c++)
        output << ", " << cornerIndexRow.Value(0, c);
    output << "]";
    for (int r = 1; r < cornerIndexRow.getRows(); r++)
    {
        output << ", [" << cornerIndexRow.Value(r, 0);
        for (int c = 1; c < cornerIndexRow.getCols(); c++)
            output << ", " << cornerIndexRow.Value(r, c);
        output << "]";
    }
    output << "];" << endl;
    
    // cornerIndexCol
    output << "cornerIndexCol = [";
    output << "[" << cornerIndexCol.Value(0, 0);
    for (int c = 1; c < cornerIndexCol.getCols(); c++)
        output << ", " << cornerIndexCol.Value(0, c);
    output << "]";
    for (int r = 1; r < cornerIndexCol.getRows(); r++)
    {
        output << ", [" << cornerIndexCol.Value(r, 0);
        for (int c = 1; c < cornerIndexCol.getCols(); c++)
            output << ", " << cornerIndexCol.Value(r, c);
        output << "]";
    }
    output << "];" << endl;
}

Plotter::~Plotter()
{
}
