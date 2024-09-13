#ifndef ALGEBRA_H_
#define ALGEBRA_H_

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <stdio.h>

#include "memory.h"
#include "log.h"

/******************************************************************************/
/*    TEST FUNCTIONS                                                          */
/******************************************************************************/

MATRIX* transpose(MATRIX *A)
{
    if (A == NULL)
    {
        LOG_ERROR("Nothing to transpose: Empty matrix!");
        return (MATRIX*) NULL;
    }

    MATRIX *AT = push_matrix(A->cols, A->rows);

    for (uint32_t row = 0U; row < A->rows; row++)
    {
        for (uint32_t col = 0U; col < A->cols; col++)
        {
            uint32_t pos = A->cols * row + col;
            uint32_t posT = A->rows * col + row;

            AT->val[posT] = A->val[pos];
        }
    }

    return AT;
}

MATRIX* add(MATRIX *A, MATRIX *B)
{
    if ((A->rows != B->rows) || (A->cols != B->cols))
    {
        LOG_ERROR("Matrices does not have suitable size for matrix-addition.");
        return (MATRIX*)NULL;
    }

    MATRIX *C = push_matrix(A->rows, A->cols);

    for (uint32_t row = 0U; row < A->rows; row++)
    {
        for (uint32_t col = 0U; col < A->cols; col++)
        {
            uint32_t pos = A->cols * row + col;

            C->val[pos] = A->val[pos] + B->val[pos];
        }
    }

    return C;
}

MATRIX* subs(MATRIX *A, MATRIX *B)
{
    if ((A->rows != B->rows) || (A->cols != B->cols))
    {
        LOG_ERROR("Matrices does not have suitable size for matrix-substraction.");
        return (MATRIX*)NULL;
    }

    MATRIX *C = push_matrix(A->rows, A->cols);

    for (uint32_t row = 0U; row < A->rows; row++)
    {
        for (uint32_t col = 0U; col < A->cols; col++)
        {
            uint32_t pos = A->cols * row + col;

            C->val[pos] = A->val[pos] - B->val[pos];
        }
    }

    return C;
}

MATRIX* mult(MATRIX *A, MATRIX *B)
{
    if (A->cols != B->rows)
    {
        LOG_ERROR("Matrices does not have suitable size for matrix-multiplication.");
        return (MATRIX*)NULL;
    }

    MATRIX *C = push_matrix(A->rows, B->cols);

    for (uint32_t row = 0U; row < A->rows; row++)
    {
        uint32_t vecA = A->cols * row;

        for (uint32_t col = 0U; col < B->cols; col++)
        {
            uint32_t pos = C->cols * row + col;
            LOG_TRACE("C[%u,%u] ~ C[%u]", row, col, pos);

            for (uint32_t i = 0; i < A->cols; i++)
            {
                uint32_t vecB = B->cols * i;
                LOG_TRACE("A[%u,%u] * B[%u,%u] := A[%u] x B[%u]", row, i, col, vecB + col, vecA + i, vecB + col);
                C->val[pos] += A->val[vecA + i] * B->val[vecB + col];
            }
        }
    }

    return C;
}

#ifdef __cplusplus
}
#endif

#endif /* ALGEBRA_H_ */
