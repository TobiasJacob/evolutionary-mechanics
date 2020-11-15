#include <iostream>
#include <iomanip>
#include "Field.hpp"
#include "PerformanceEvaluator.hpp"

using namespace std;

int main()
{
    Field field(4, 4);
    field.block(1, 1) = true;
    field.block(2, 1) = true;
    PerformanceEvaluator evaluator;
    cout << evaluator.getPerformance(field) << endl;
}

