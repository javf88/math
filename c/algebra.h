/*******************************************************************************
*
* Matrix Algebra
*
*   SUMMARY
*       This single-header module implements some very well-known routines
*       some of them are the follwing:
*
*       - matrix transpose, A^T
*       - matrix additions and substraction, A +/- B
*       - matrix multiplication, A x B
*
*******************************************************************************/

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
#include <float.h>
#include <math.h>

#include "memory.h"
#include "log.h"

/******************************************************************************/
/*    PRIVATE DATA AND IMPLEMENTATION                                         */
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

MATRIX* id(uint32_t size)
{
    MATRIX *A = push_matrix(size, size);
    if (A == NULL)
    {
        LOG_ERROR("Identity matrix was not created.");
        return NULL;
    }

    for (uint32_t i = 0U; i < A->rows; i++)
    {
        A->val[A->cols * i + i] = 1.0F;
    }

    return A;
}

uint32_t get_row(MATRIX *A, uint32_t j)
{
    for (uint32_t pos = j + 1U; pos < A->rows; pos++)
    {
        if (FLT_EPSILON < fabs(A->val[A->cols * pos + j]))
        {
            /* let us return the next row with the pivot*/
            return pos;
        }
    }

    /* returning first index that is out of range (+1) */
    return A->rows;
}

MATRIX* permute(MATRIX *I, uint32_t a, uint32_t b)
{
    if (I->rows <= a || I->rows <= b)
    {
        LOG_ERROR("Permutation indecis(%u,%u) are out of range(Id[%u])", a, b, I->rows);
        return I;
    }

    /* permuting(swapping) I[a,*] with I[b,*] rows */
    float *posA = &I->val[I->cols * a + a];
    float *posB = &I->val[I->cols * b + b];

    float *newA = &I->val[I->cols * a + b];
    float *newB = &I->val[I->cols * b + a];

    posA[0] = posB[0] = 0.0F;
    newA[0] = newB[0] = 1.0F;

    return I;
}

MATRIX* update(MATRIX *A, uint32_t j)
{
    float *pivot = &A->val[A->cols * j + j];
    uint32_t newJ = 0U;
    MATRIX *P = NULL;

    /* no-need to update A, pivot is non-zero */
    if (fabs(pivot[0]) > FLT_EPSILON)
    {
        return A;
    }

    /* get row of next non-zero value */
    newJ = get_row(A, j);
    LOG_INFO("newJ = %u", newJ);
    if (newJ < A->rows)
    {
        /* Build Identity and swap rows */
        P = id(A->rows);
        P = permute(P, j, newJ);
        LOG_INFO_MATRIX(P);

        /* Row-permutation, left-multiplication, on A */
        A = mult(P, A);
        LOG_INFO_MATRIX(A);
    }

    return A;
}

MATRIX* echelon(MATRIX *A)
{
    if (A->cols != A->rows)
    {
        LOG_ERROR("Matrix is not a square matrix.");

        return NULL;
    }

    for (uint32_t j = 0U; j < (A->cols - 1U); j++)
    {
        /* Let us create an Identity matrix */
        MATRIX *L = id(A->cols);
        float *pivot = NULL;

        /* Let us update A by finding and permuting the next non-zero val 
         * in j-th col */
        A = update(A, j);
        pivot = &A->val[A->cols * j + j];
        LOG_INFO("pivot: %f", *pivot);

        /* If pivot is zero, then it is a singular matrix */
        if (fabs(pivot[0]) <= FLT_EPSILON)
        {
            /* Removing unused matrix L_(j) */
            stack = pop_matrix(stack);
            return A;
        }

        for (uint32_t i = j + 1U; i < A->rows; i++)
        {
            /* Let us place the right coef under the pivot (a[i,i]) */
            float a = A->val[A->cols * i + j];

            /* If pivot is numerically zero, the rest of the column should
             * tested for zeros. If they are all zero, then the matrix is singular. */
            LOG_INFO("a: %.7f", a);
            L->val[A->cols * i + j] = -1.0F * a / pivot[0];
        }

        /* let us remove the current column under the pivot */
        MATRIX *B = mult(L, A);

        LOG_INFO_MATRIX(L);
        LOG_INFO_MATRIX(A);
        LOG_INFO_MATRIX(B);

        /* Update the matrix for the next iteration */
        A = B;
    }

    return A;
}

#ifdef __cplusplus
}
#endif

#endif /* ALGEBRA_H_ */
