/*******************************************************************************
*
* Matrix class
*
*   SUMMARY
*       Implementation of a matrix in 2D.
*
*       The API's functionality is simple:
*       a) matrix creation
*       b) matrix destruction
*       c) reshape, transpose and identity matrix
*
*******************************************************************************/

#ifndef MATRIX_H_
#define MATRIX_H_

/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <cstdint>
#include <initializer_list>
#include <iomanip>
#include <string>
#include <vector>

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
    #define LOG_DEBUG(A.logMatrix, ...)
#endif

/******************************************************************************/
/*    API                                                                     */
/******************************************************************************/

struct Matrix
{
    Memory<Matrix*> manager;

    // Logging capabilities
    Log logMatrix;
    std::string name = "A";

    uint32_t    rows = 0;
    uint32_t    cols = 0;
    std::vector<float>  val;

    // Constructors & Destructors
    // Memory allocation for a matrix
    Matrix(uint32_t rows, uint32_t cols);

    // would this be private?
    Matrix(Matrix& A);

    // Empty matrix
    Matrix();

    // to move to implementation section
    ~Matrix();

    // Matrix allocation from a list
    Matrix(std::initializer_list<float> val);

    // Matrix operators to manipulate dimensions and memory layout.
    Matrix& reshape(const uint32_t newRows, const uint32_t newCols);

    Matrix& transpose();

    Matrix& id(const size_t size);

    Matrix* getBlock(const uint32_t row, const uint32_t rowEnd,
                     const uint32_t col, const uint32_t colEnd);

    void* operator new(std::size_t count);

    void operator delete(void* ptr) noexcept;

    // log is the public API
    // log(string newName) calls log()
    void log(const std::string &newName);

    // log() calls repeately log(vector<float>::c_itr row)
    // to format the whole of the matrix with N+1 Cols
    // A = [col0 | col1 | col2 | ... | colN]
    Log* log() const;

    Log* log(const std::vector<float>::const_iterator row) const;
};
#endif /* MATRIX_H_ */
