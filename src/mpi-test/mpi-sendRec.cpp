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
    vector<byte> buffer(Organism::getSize(10, 10));

    if (world_rank == 0) {
        orgA.writeIntoBuffer(buffer.data());
        MPI_Send(buffer.data(), 1, orgDtype, 1, 0, MPI_COMM_WORLD);
    } else if (world_rank == 1) {
        MPI_Recv(buffer.data(), 1, orgDtype, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        Organism orgB(10, 10);
        orgB.readFromBuffer(buffer.data());

        bool testSucceed = orgA.loss == orgB.loss;
        for (size_t r = 0; r < orgA.field->Rows; r++)
            for (size_t c = 0; c < orgA.field->Cols; c++)
                if (orgA.field->Plane(r, c) != orgB.field->Plane(r, c))
                    testSucceed = false;
        
        if(testSucceed) cout << "test succeeded" << endl;
        else cout << "test failed" << endl;
    }
    MPI_Finalize();
}