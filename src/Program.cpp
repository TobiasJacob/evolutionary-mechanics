#include <iostream>
#include <iomanip>
#include "Field.hpp"

using namespace std;

int main()
{
    Field field(4, 4);
    field.block(1, 1) = true;
}

