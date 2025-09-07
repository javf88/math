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

#include <cfloat>
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

    Matrix* setBlock(Matrix *S);

    Matrix* echelon();

    Matrix* rowPermute()
    {
        Matrix *PA = nullptr;

        uint32_t row = 0U;
        for (row = 0U; row < this->rows; row++)
        {
            auto entry = this->val.cbegin() + (this->cols * row);
            if (std::abs(*entry) > 2.0F * FLT_EPSILON)
            {
                LOG_DEBUG(this->logMatrix, "Pivot is in row ", row);
                break;
            }
        }

        if (0U < row)
        {
            if (row < this->rows)
            {
                Matrix P;
                P.id(this->rows);

                P.val[0U] = 0.0F;
                P.val[row] = 1.0F;
                P.val[this->cols * row] = 1.0F;
                P.val[this->cols * row + row] = 0.0F;
                LOG_MATRIX(P);

                //to split hpp into hpp and cpp
                PA = P * *this;
                // which matrix to return when this is made a function?
                LOG_MATRIX(*PA);
            }
            else
            {
                LOG_DEBUG(this->logMatrix, "The matrix is singular, nothing to permute.");
                PA = nullptr;
            }
        }
        else
        {
            LOG_DEBUG(this->logMatrix, "Nothing to permute (A = PA => P = I).");
            PA = this;
        }

        return PA;
    }

    Matrix* rowReduction()
    {
        Matrix L_inv;
        L_inv.id(this->rows);

        auto a = this->val.cbegin(); // A[0,0]
        for (uint32_t row = 1U; row < this->rows; row++)
        {
            uint32_t pos = this->cols * row;
            auto l = L_inv.val.begin() + pos; // L^{-1}[i,0]
            auto b = this->val.cbegin() + pos; // A[i,0]

            *l = *b / *a * -1.0F;
        }
        LOG_MATRIX(L_inv);

        // PA = LU => L^{-1} PA = U
        Matrix *U = L_inv * *this;
        LOG_MATRIX(*U);

        return U;
    }

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

    friend Matrix* operator*(Matrix &A, Matrix &B);
};
#endif /* MATRIX_H_ */
