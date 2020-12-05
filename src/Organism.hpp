#ifndef EVOLUTIONARYORGANISM
#define EVOLUTIONARYORGANISM

#include <vector>
#include <optional>
#include <mpi.h>
#include "Matrix.hpp"
#include "Field.hpp"

struct Organism { // float loss, size_t row, size_t cols, bool[row * col] dim
    static unique_ptr<MPI_Datatype> mpiDatatype;
    float loss = 0.0;
    unique_ptr<Field> field;
    Organism(size_t rows, size_t cols);
    // Organism is copyable
    Organism(Organism const &other);
    Organism &operator= (Organism const &other);
    Organism(Organism &&other);
    Organism &operator= (Organism &&other);
    size_t countPlanes();
    static MPI_Datatype &getDatatype(size_t rows, size_t cols);
    static size_t getSize(size_t rows, size_t cols);
    void writeIntoBuffer(void *buffer);
    void readFromBuffer(void *buffer);
};
#endif