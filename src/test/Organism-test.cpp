#include "catch/catch.hpp"

#include "../Organism.hpp"

TEST_CASE("OrganismEncodeAndDecode", "[Equation]")
{
    srand(0);
    Organism orgA(10, 10);
    Organism orgB(10, 10);

    orgA.loss = 22321.32;
    for (size_t r = 0; r < orgA.field->Rows; r++)
        for (size_t c = 0; c < orgA.field->Cols; c++)
            orgA.field->Plane(r, c) = rand(0) % 2;
        
    vector<byte> buffer(Organism::getSize(10, 10));
    orgA.writeIntoBuffer(buffer.data());
    orgB.readFromBuffer(buffer.data());

    REQUIRE(orgA.loss == orgB.loss);
    for (size_t r = 0; r < orgA.field->Rows; r++)
        for (size_t c = 0; c < orgA.field->Cols; c++)
            REQUIRE(orgA.field->Plane(r, c) == orgB.field->Plane(r, c));
}