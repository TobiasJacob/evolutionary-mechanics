#include "catch/catch.hpp"

#include "../SparseMatrix.hpp"

TEST_CASE("SparseMatrixDefaultValue", "[SparseMatrix]")
{
    const int defaultValue = 321;
    SparseMatrix<int> matrix(120, 302, defaultValue);
    REQUIRE(matrix.GetValue(0, 0) == defaultValue);
    REQUIRE(matrix.GetValue(23, 40) == defaultValue);
    REQUIRE(matrix.GetValue(23, 40) == defaultValue);
    REQUIRE(matrix.GetValue(119, 301) == defaultValue);

    const float defautFloat = 1.2321e2;
    SparseMatrix<float> matrixFloat(120, 302, defautFloat);
    REQUIRE(matrixFloat.GetValue(119, 301) == defautFloat);
}

TEST_CASE("SparseMatrixOutOfBoundException", "[SparseMatrix]")
{
    SparseMatrix<float> matrix(3, 6, 0);
    REQUIRE_THROWS(matrix.GetValue(0, -1));
    REQUIRE_THROWS(matrix.GetValue(2, 7));
    REQUIRE_THROWS(matrix.GetValue(3, 0));
    REQUIRE_THROWS(matrix.GetValue(1313, 1232));
}

TEST_CASE("SparseMatrixInsert", "[SparseMatrix]")
{
    float defaultValue = 0.1;
    SparseMatrix<float> matrix(314, 221, defaultValue);

    int testRows[4] = {123, 0, 14, 313};
    int testCols[3] = {0, 103, 220};
    float value = -302.10e-2;
    for (const int row : testRows)
    {
        for (const int col : testCols)
        {
            INFO("Testing " << row << ", " << col);
            REQUIRE(matrix.GetValue(row, col) == defaultValue);
            matrix.SetValue(row, col, value);
            REQUIRE(matrix.GetValue(row, col) == value);
            value += 1;
        }
    }

    matrix.SetValue(100, 101, value);
    REQUIRE(matrix.GetValue(100, 100) == defaultValue);
    REQUIRE(matrix.GetValue(100, 101) == value);
    REQUIRE(matrix.GetValue(100, 102) == defaultValue);
    matrix.SetValue(100, 100, value);
    matrix.SetValue(100, 102, value);
    REQUIRE(matrix.GetValue(100, 100) == value);
    REQUIRE(matrix.GetValue(100, 101) == value);
    REQUIRE(matrix.GetValue(100, 102) == value);
}
