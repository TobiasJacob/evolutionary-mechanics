#include "Equation.hpp"

Equation::Equation(int conditions, int unknowns) : Conditions(conditions), Unknowns(unknowns), K(conditions, unknowns, 0), f(conditions, 0)
{
    
}

unique_ptr<vector<float>> Equation::SolveIterative() 
{
    // TODO Tobi: Is currently not working
    unique_ptr<vector<float>> currentSolution = make_unique<vector<float>>(Unknowns, 0);
    unique_ptr<vector<float>> nextSolution = make_unique<vector<float>>(Unknowns, 0);
    float delta = 1.;
    float alpha = 0.25;
    int counter = 0;
    cout << setprecision(8);
    while (delta > 0.001 && counter < 10000)
    {
        for (int r = 0; r < Conditions; r++)
        {
            (*nextSolution)[r] = 0;
            for (int c = 0; c < Unknowns; c++)
                (*nextSolution)[r] += K.Value(r, c) * (*currentSolution)[c];            
        }
        // cout << "f_ ";
        // for (float _f: *nextSolution)
        //     cout << _f << " ";
        // cout << endl;

        // cout << "delta_ ";
        delta = 0;
        for (int r = 0; r < Conditions; r++)
        {
            const float curDelta = (f[r] - (*nextSolution)[r]);
            (*nextSolution)[r] = (*currentSolution)[r] - alpha * curDelta;
            delta += abs(curDelta);
            // cout << curDelta << " ";
        }
        // cout << endl;
        // cout << "epsilon_ ";
        // for (float _f: *nextSolution)
        //     cout << _f << " ";
        // cout << endl;
        // cout << delta << " ";
        currentSolution.swap(nextSolution);
        counter++;
    }
    cout << "f ";
    for (float _f: *nextSolution)
        cout << _f << " ";
    cout << endl;
    cout << counter << "," << delta << endl;
    for (int r = 0; r < Conditions; r++)
    {
        (*nextSolution)[r] = 0;
        for (int c = 0; c < Unknowns; c++)
            (*nextSolution)[r] += K.Value(r, c) * (*currentSolution)[c];            
    }
    cout << "f_ ";
    for (float _f: *nextSolution)
        cout << _f << " ";
    cout << endl;

    return currentSolution;
}

void Equation::Print() 
{
    K.Print();
    for (float _f: f)
        cout << _f << endl;
}