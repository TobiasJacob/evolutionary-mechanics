#include "catch/catch.hpp"

#include "../SparseMatrix.hpp"

TEST_CASE("SparseMatrixDefaultValue", "[SparseMatrix]")
{
    SparseMatrix<int> matrix(120, 302);
    REQUIRE(matrix.GetValue(0, 0) == 0);
    REQUIRE(matrix.GetValue(23, 40) == 0);
    REQUIRE(matrix.GetValue(23, 40) == 0);
    REQUIRE(matrix.GetValue(119, 301) == 0);

    SparseMatrix<float> matrixFloat(120, 302);
    REQUIRE(matrixFloat.GetValue(119, 301) == 0);
}

TEST_CASE("SparseMatrixOutOfBoundException", "[SparseMatrix]")
{
    SparseMatrix<float> matrix(3, 6);
    REQUIRE_THROWS(matrix.GetValue(0, -1));
    REQUIRE_THROWS(matrix.GetValue(2, 7));
    REQUIRE_THROWS(matrix.GetValue(3, 0));
    REQUIRE_THROWS(matrix.GetValue(1313, 1232));
}

TEST_CASE("SparseMatrixInsert", "[SparseMatrix]")
{
    SparseMatrix<float> matrix(314, 221);

    int testRows[4] = {123, 0, 14, 313};
    int testCols[3] = {0, 103, 220};
    float value = -302.10e-2;
    for (const int row : testRows)
    {
        for (const int col : testCols)
        {
            INFO("Testing " << row << ", " << col);
            REQUIRE(matrix.GetValue(row, col) == 0);
            matrix.SetValue(row, col, value);
            REQUIRE(matrix.GetValue(row, col) == value);
            value += 1;
        }
    }

    matrix.SetValue(100, 101, value);
    REQUIRE(matrix.GetValue(100, 100) == 0);
    REQUIRE(matrix.GetValue(100, 101) == value);
    REQUIRE(matrix.GetValue(100, 102) == 0);
    matrix.SetValue(100, 100, value);
    matrix.SetValue(100, 102, value);
    REQUIRE(matrix.GetValue(100, 100) == value);
    REQUIRE(matrix.GetValue(100, 101) == value);
    REQUIRE(matrix.GetValue(100, 102) == value);
}

TEST_CASE("SparseMatrixGetOrAllocate", "[SparseMatrix]")
{
    SparseMatrix<float> matrix(314, 221);

    int testRows[4] = {123, 0, 14, 313};
    int testCols[3] = {0, 103, 220};
    float value = -302.10e-2;
    for (const int row : testRows)
    {
        for (const int col : testCols)
        {
            INFO("Testing " << row << ", " << col);
            REQUIRE(matrix.GetValue(row, col) == 0);
            matrix.GetOrAllocateValue(row, col) = value;
            REQUIRE(matrix.GetValue(row, col) == value);
            value += 1;
        }
    }

    matrix.GetOrAllocateValue(100, 101) = value;
    REQUIRE(matrix.GetValue(100, 100) == 0);
    REQUIRE(matrix.GetValue(100, 101) == value);
    REQUIRE(matrix.GetValue(100, 102) == 0);
    matrix.GetOrAllocateValue(100, 100) = value;
    matrix.GetOrAllocateValue(100, 102) = value;
    REQUIRE(matrix.GetValue(100, 100) == value);
    REQUIRE(matrix.GetValue(100, 101) == value);
    REQUIRE(matrix.GetValue(100, 102) == value);
}

TEST_CASE("SparseMatrixMultiplication", "[SparseMatrix]")
{
    SparseMatrix<float> matrix(5, 8);

    vector<float> vec1 = {0, 1, 0, 0, 2, 0, -4, 0};
    vector<float> vec2 = {0, 1, 0, 0, 2, 0, -4, 3, 0};
    vector<float> result2 = {0, 0, 0, 0, 0};
    vector<float> result1 = {0, 0, 0, 0, 0};

    REQUIRE_THROWS(matrix.Multiply(vec2, result2));

    SECTION( "multiply only with default values" ) {
        matrix.Multiply(vec1, result1);
        vector<float> result1Require = {0, 0, 0, 0, 0};
        for (size_t i = 0; i < 5; i++)
        {
            INFO(result1[i] << " == " << result1Require[i]);
            REQUIRE(result1[i] == Approx(result1Require[i]).margin(1e-30));
        }
    }

    SECTION( "multiply only with default values" ) {
        matrix.SetValue(0, 0, 1);
        matrix.SetValue(0, 1, 1);
        matrix.SetValue(1, 1, 1);
        matrix.SetValue(1, 4, 1);
        matrix.SetValue(1, 6, 1);
        matrix.SetValue(2, 1, 2);
        matrix.SetValue(2, 4, -1);
        matrix.SetValue(3, 0, 2);
        matrix.SetValue(3, 2, -24);
        matrix.SetValue(3, 3, 23);
        matrix.SetValue(3, 5, 1324);
        matrix.SetValue(3, 7, -1343);
        vector<float> result = {0, 0, 0, 0, 0};
        matrix.Multiply(vec1, result);
        vector<float> resultRequire = {1, -1, 0, 0, 0};
        for (size_t i = 0; i < 5; i++)
        {
            INFO(result[i] << " == " << resultRequire[i]);
            REQUIRE(result[i] == Approx(resultRequire[i]).margin(1e-30));
        }
    }

}
