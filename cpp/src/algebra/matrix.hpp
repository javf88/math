/*******************************************************************************
*
* Matrix class
*
*   SUMMARY
*       Implementation of a matrix in 2D.
*
*       The API's functionality is simple:
*       a) matrix creation and destruction,
*       b) memory management,
*       c) minimal set of matrix operators to manipulate matrices
*       d) logging capabilities
*
*******************************************************************************/

#ifndef MATRIX_H_
#define MATRIX_H_

/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include "levels.hpp"
#include "memory.hpp"

/******************************************************************************/
/*    PUBLIC MACROS                                                           */
/******************************************************************************/

/**
 * @brief   Macro to get a row-vector from a matrix.
 */
#define GET_ROW_VECTOR(A, row) \
    (A).getBlock(row, row + 1U, 0U, (A).cols)

/**
 * @brief   Macro to get a column-vector from a matrix.
 */
#define GET_COLUMN_VECTOR(A, col) \
    (A).getBlock(0U, (A).rows, col, col + 1U)

/**
 * @example    LOG_MATRIX(A);
 */
#if LOG_LEVEL_DEBUG <= LOG_CONFIG
    #define LOG_MATRIX(A) \
        LOG_DEBUG((A).logMatrix, #A, " in [", (A).rows, "x", (A).cols, "]."); \
        (A).log(#A);
#else
    #define LOG_MATRIX(A)
#endif

/******************************************************************************/
/*    API                                                                     */
/******************************************************************************/

struct Matrix
{
    // Memory management
    Memory<Matrix*> manager;

    // Logging capabilities
    Log logMatrix;
    std::string name = "A";

    // Matrix abstraction
    uint32_t    rows = 0;
    uint32_t    cols = 0;
    std::vector<float>  val;

    // Constructors & Destructors
    Matrix(std::initializer_list<float> val); // Matrix allocation from a list
    Matrix(uint32_t rows, uint32_t cols);     // Memory allocation for a matrix
    Matrix(Matrix& A);                        // would this be private?
    Matrix();                                 // Empty matrix
    ~Matrix();


    // Matrix operators to manipulate dimensions and memory layout.
    Matrix& reshape(const uint32_t newRows, const uint32_t newCols);
    Matrix& transpose();
    Matrix& id(const size_t size);
    // Echelon specific functions
    Matrix* getBlock();
    Matrix* setBlock(Matrix *S);
    Matrix* rowPermute();
    Matrix* rowReduction();
    Matrix* echelon();

    // Glue code for the memory management
    void* operator new(std::size_t count);
    void operator delete(void* ptr) noexcept;

    // log is the public API
    // log(string newName) calls log()
//    void log(const std::string &newName);
    // log() calls repeately log(vector<float>::c_itr row)
    // to format the whole of the matrix with N+1 Cols
    // A = [col0 | col1 | col2 | ... | colN]
//    Log* log() const;
    std::string log(const std::vector<float>::const_iterator row) const;

    // When removing const, googletest complains
    friend bool operator==(const Matrix& A, const Matrix& B);
    friend Matrix* operator+(Matrix& A, Matrix& B);
    // same case as with + operator
    friend Matrix* operator-(Matrix& A, Matrix &B);
    friend Matrix* operator*(Matrix& A, Matrix &B);
    // implicit conversion from ints to floats
    friend Matrix* operator*(const float a, Matrix &B);
    friend Matrix* operator*(Matrix &A, const float b);
    friend Matrix* operator*(Matrix &A, const float b);
};
#endif /* MATRIX_H_ */
