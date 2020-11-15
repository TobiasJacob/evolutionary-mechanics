#include <iostream>
#include <iomanip>
#include "Field.hpp"

using namespace std;

int main()
{
    Field field(4, 4);
    unique_ptr<Equation> equation = field.buildEquation();
    equation->print();
    equation->solveIterative();
}

