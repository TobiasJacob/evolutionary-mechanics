#include "Plotter.hpp"

Plotter::Plotter(string fileName): output(fileName)
{
    std::ifstream f("src/plotting/Plotter.html");
    std::string str;

    f.seekg(0, std::ios::end);   
    str.reserve(f.tellg());
    f.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(f)),
                std::istreambuf_iterator<char>());

    head = str.substr(0, str.find("// Paste output here"));
    tail = str.substr(str.find("// End Paste output here"));
}

// Requires not empty fields, etc.
void Plotter::plot(const Field &field, const vector<float> &u, const Matrix<int> &cornerIndexRow, const Matrix<int> &cornerIndexCol, const Support &supports, const vector<Force> &forces, const int solutionSteps, const float residum, const vector<float> &stress, const float time) 
{
    // head
    output << head << endl;

    // field
    output << "const field = [";
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

    // u, prepend a leading zero
    output << "const u = [" << 0;
    for (int i = 0; i < u.size(); i++)
        output << ", " << u[i];
    output << "];" << endl;

    
    // cornerIndexRow
    output << "const cornerIndexRow = [";
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
    output << "const cornerIndexCol = [";
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

    // supports
    output << "const supports = { RowSupports: [";
    output << "{ row: " << supports.RowSupports[0].row << ", col: " << supports.RowSupports[0].col << " }";
    for (int i = 1; i < supports.RowSupports.size(); i++)
        output << ", { row: " << supports.RowSupports[i].row << ", col: " << supports.RowSupports[i].col << " }";
    output << "], ColSupports: [";
    output << "{ row: " << supports.ColSupports[0].row << ", col: " << supports.ColSupports[0].col << " }";
    for (int i = 1; i < supports.ColSupports.size(); i++)
        output << ", { row: " << supports.ColSupports[i].row << ", col: " << supports.ColSupports[i].col << " }";
    output << "]}" << endl;

    // forces
    output << "const forces = [";
    output << "{ attackCorner: { row: " << forces[0].attackCorner.row << ", col: " << forces[0].attackCorner.col << "}, "
            << "forceRow: " << forces[0].forceRow << ", forceCol:" << forces[0].forceCol << "}";
    for (int i = 1; i < forces.size(); i++)
    {
        output << ", { attackCorner: { row: " << forces[i].attackCorner.row << ", col: " << forces[i].attackCorner.col << "}, "
                << "forceRow: " << forces[i].forceRow << ", forceCol:" << forces[i].forceCol << "}";
    }
    output << "]" << endl;
    
    // solutionSteps
    output << "const solutionSteps = " << solutionSteps << ";" << endl;

    // residum
    output << "const residum = " << residum << ";" << endl;

    // stress
    output << "const stress = [" << stress[0];
    for (int i = 1; i < stress.size(); i++)
        output << ", " << stress[i];
    output << "];" << endl;

    // residum
    output << "const time = " << time << ";" << endl;

    // tail
    output << tail;
}

Plotter::~Plotter()
{
}
