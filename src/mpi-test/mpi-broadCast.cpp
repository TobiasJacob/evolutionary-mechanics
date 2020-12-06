#include <mpi.h>
#include <iostream>

#include "../Organism.hpp"

using namespace std;

int main(int argc, char **argv)
{
    // MPI Stuff
    MPI_Init(&argc, &argv);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Datatype orgDtype = Organism::getDatatype(10, 10);

    // Setup organism
    srand(0);
    Organism orgA(10, 10);

    orgA.loss = 22321.32;
    for (size_t r = 0; r < orgA.field->Rows; r++)
        for (size_t c = 0; c < orgA.field->Cols; c++)
            orgA.field->Plane(r, c) = rand() % 2;
    
    // Setup buffer
    size_t orgSize = Organism::getSize(10, 10);
    vector<byte> buffer(orgSize * 2);
    
    if (world_rank == 0) {
        orgA.writeIntoBuffer(buffer.data());
        orgA.writeIntoBuffer(buffer.data() + orgSize);
    }
    MPI_Bcast(buffer.data(), 2, orgDtype, 0, MPI_COMM_WORLD);
    
    bool testSucceed = true;
    Organism orgB(10, 10);
    for (byte *offset: { buffer.data(), buffer.data() + orgSize })
    {
        orgB.readFromBuffer(offset);

        testSucceed = testSucceed && (orgA.loss == orgB.loss);
        for (size_t r = 0; r < orgA.field->Rows; r++)
            for (size_t c = 0; c < orgA.field->Cols; c++)
                if (orgA.field->Plane(r, c) != orgB.field->Plane(r, c))
                    testSucceed = false;
    }

    if(testSucceed) cout << "test succeeded" << endl;
    else cout << "test failed" << endl;
    MPI_Finalize();
}