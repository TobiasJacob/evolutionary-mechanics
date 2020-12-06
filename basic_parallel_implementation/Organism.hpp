/**
 * @file Organism.hpp
 * @author Tobias Jacob - Raffaele Galliera - Ali Muddasar
 * @brief 
 * @version 1.0
 * @date 2020-12-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef EVOLUTIONARYORGANISM
#define EVOLUTIONARYORGANISM

#include <vector>
#include <optional>
#include <mpi.h>
#include "Matrix.hpp"
#include "Field.hpp"

/**
 * @brief This class defines an abstraction of what represents an Organisms. Mainly used by the Evolutionary Optimizator.
 * 
 */
struct Organism { // float loss, size_t row, size_t cols, bool[row * col] dim
    static MPI_Datatype mpiDatatype;
    float loss = 0.0;
    unique_ptr<Field> field;
    Organism(size_t rows, size_t cols);
    // Organism is copyable
    Organism(Organism const &other);
    Organism &operator= (Organism const &other);
    Organism(Organism &&other);
    Organism &operator= (Organism &&other);
    size_t countPlanes();
    /**
     * @brief Get the Datatype object
     * Define and returns MPI datatype used for serializing the organism
     * @param rows 
     * @param cols 
     * @return MPI_Datatype 
     */
    static MPI_Datatype getDatatype(size_t rows, size_t cols);
    /**
     * @brief Get the Size object
     * 
     * @param rows 
     * @param cols 
     * @return size_t 
     */
    static size_t getSize(size_t rows, size_t cols);
    /**
     * @brief Write the Organism fields into the buffer
     * 
     * @param buffer 
     */
    void writeIntoBuffer(void *buffer);
    /**
     * @brief Set the Organism object with the fields read from the buffer
     * 
     * @param buffer 
     */
    void readFromBuffer(void *buffer);
};
#endif