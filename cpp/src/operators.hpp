/*******************************************************************************
*
* Operators
*
*   SUMMARY
*
*******************************************************************************/

#ifndef OPERATORS_H_
#define OPERATORS_H_

/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <cstdint>
#include <cstdio>

#include "matrix.hpp"

/******************************************************************************/
/*    API                                                                     */
/******************************************************************************/

Matrix& operator+(const Matrix& A, const Matrix& B)
{
    Matrix *C = nullptr;

    if ((A.rows != B.rows) || (A.cols != B.cols))
    {
        std::fprintf(stderr, "Matrices A[%hu,%hu] and B[%hu,%hu] cannot be added.",
                A.rows, A.cols, B.rows,B.cols);

    }
    else
    {
        C = new Matrix(A.rows, A.cols);

        for (uint32_t i = 0U; i < A.rows; i++)
        {
            for (uint32_t j = 0U; j < A.cols; j++)
            {
                uint32_t pos = C->cols * i + j;
                C->val[pos] = A.val[pos] + B.val[pos];
            }
        }
    }

    return *C;
}

#endif /* OPERATORS_H_ */
