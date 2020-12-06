#include "Organism.hpp"

Organism::Organism(size_t rows, size_t cols)
    : field(new Field(rows, cols, true))
{
    
}

Organism::Organism(Organism const &other)
    : loss(other.loss), field(new Field(other.field->Rows, other.field->Cols))
{
    for (size_t r = 0; r < field->Rows; r++)
        for (size_t c = 0; c < field->Cols; c++)
            field->Plane(r, c) = other.field->Plane(r, c);    
}

Organism &Organism::operator= (Organism const &other)
{
    #ifdef DEBUG
    if (field->Rows != other.field->Rows || field->Cols != other.field->Cols) throw "Fields have different sizes";
    #endif
    loss = other.loss;
    for (size_t r = 0; r < field->Rows; r++)
        for (size_t c = 0; c < field->Cols; c++)
            field->Plane(r, c) = other.field->Plane(r, c);    
    return *this;
}

Organism::Organism(Organism &&other) 
{
    field = move(other.field);
    loss = other.loss;
}

Organism& Organism::operator= (Organism &&other) 
{
    if (this != &other) 
    {
        field = move(other.field);
        loss = other.loss;
    }
    return *this;
}

size_t Organism::countPlanes() 
{
    size_t count = 0;
    for (size_t r = 0; r < field->Rows; r++)
        for (size_t c = 0; c < field->Cols; c++)
            if (field->Plane(r, c)) count++;
    return count;
}

MPI_Datatype Organism::mpiDatatype = 0;

MPI_Datatype Organism::getDatatype(size_t rows, size_t cols) 
{
    if (!mpiDatatype)
    {
        int lengths[2] = {1, (int)rows * (int)cols};
        MPI_Aint displacements[2] = {0, sizeof(float)};
        MPI_Datatype types[2] = {MPI_FLOAT, MPI_C_BOOL};
        MPI_Type_create_struct(2, lengths, displacements, types, &mpiDatatype);
        MPI_Type_commit(&mpiDatatype);
    }
    return mpiDatatype;
}

size_t Organism::getSize(size_t rows, size_t cols)
{
    return sizeof(float) + rows * cols * sizeof(bool);
}

void Organism::writeIntoBuffer(void *buffer)
{
    float *lossPtr = (float *)buffer;
    bool *fieldPtr = (bool *)(lossPtr + 1);
    *lossPtr = loss;
    for (size_t r = 0; r < field->Rows; r++)
        for (size_t c = 0; c < field->Cols; c++)
            fieldPtr[r * field->Cols + c] = field->Plane(r, c);
}

void Organism::readFromBuffer(void *buffer)
{
    float *lossPtr = (float *)buffer;
    bool *fieldPtr = (bool *)(lossPtr + 1);
    loss = *lossPtr;
    for (size_t r = 0; r < field->Rows; r++)
        for (size_t c = 0; c < field->Cols; c++)
            field->Plane(r, c) = fieldPtr[r * field->Cols + c];
}
