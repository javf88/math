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
#include "levels.h"

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
    MATRIX *I = push_matrix(size, size);
    if (I == NULL)
    {
        LOG_ERROR("Identity matrix was not created.");
        return NULL;
    }

    for (uint32_t i = 0U; i < I->rows; i++)
    {
        I->val[I->cols * i + i] = 1.0F;
    }

    return I;
}

uint32_t get_row(MATRIX *U, uint32_t j)
{
    for (uint32_t pos = j + 1U; pos < U->rows; pos++)
    {
        if (FLT_EPSILON < fabs(U->val[U->cols * pos + j]))
        {
            /* let us return the next row with the pivot*/
            return pos;
        }
    }

    /* returning first index that is out of range (+1) */
    return U->rows;
}

MATRIX* permute(MATRIX *P, uint32_t a, uint32_t b)
{
    if (P->rows <= a || P->rows <= b)
    {
        LOG_ERROR("Permutation indecis(%u,%u) are out of range(Id[%u])", a, b, P->rows);
        return P;
    }

    /* permuting(swapping) I[a,*] with I[b,*] rows */
    float *posA = &P->val[P->cols * a + a];
    float *posB = &P->val[P->cols * b + b];

    float *newA = &P->val[P->cols * a + b];
    float *newB = &P->val[P->cols * b + a];

    posA[0] = posB[0] = 0.0F;
    newA[0] = newB[0] = 1.0F;

    return P;
}

MATRIX* update(MATRIX *U, uint32_t j)
{
    float *pivot = &U->val[U->cols * j + j];
    uint32_t newJ = 0U;
    MATRIX *P = NULL;

    /* no-need to update U, pivot is non-zero */
    if (fabs(pivot[0]) > FLT_EPSILON)
    {
        return U;
    }

    /* get row of next non-zero value */
    newJ = get_row(U, j);
    LOG_INFO("newJ = %u", newJ);
    if (newJ < U->rows)
    {
        /* Build Identity and swap rows */
        P = id(U->rows);
        P = permute(P, j, newJ);
        LOG_INFO_MATRIX(P);

        /* Row-permutation, left-multiplication, on U */
        U = mult(P, U);
        LOG_INFO_MATRIX(U);
    }

    return U;
}

MATRIX* echelon(MATRIX *U)
{
    if (U->cols != U->rows)
    {
        LOG_ERROR("Matrix is not a square matrix.");

        return NULL;
    }

    for (uint32_t j = 0U; j < (U->cols - 1U); j++)
    {
        /* Let us create an Identity matrix */
        MATRIX *L = id(U->cols);
        float *pivot = NULL;

        /* Let us update A by finding and permuting the next non-zero val 
         * in j-th col */
        U = update(U, j);
        pivot = &U->val[U->cols * j + j];
        LOG_INFO("pivot: %f", *pivot);

        /* If pivot is zero, then it is a singular matrix */
        if (fabs(pivot[0]) <= FLT_EPSILON)
        {
            /* Removing unused matrix L_(j) */
            stack = pop_matrix(stack);
            return U;
        }

        for (uint32_t i = j + 1U; i < U->rows; i++)
        {
            /* Let us place the right coef under the pivot (a[i,i]) */
            float a = U->val[U->cols * i + j];

            /* If pivot is numerically zero, the rest of the column should
             * tested for zeros. If they are all zero, then the matrix is singular. */
            LOG_INFO("a: %.7f", a);
            L->val[U->cols * i + j] = -1.0F * a / pivot[0];
        }

        /* let us remove the current column under the pivot */
        MATRIX *B = mult(L, U);

        LOG_INFO_MATRIX(L);
        LOG_INFO_MATRIX(U);
        LOG_INFO_MATRIX(B);

        /* Update the matrix for the next iteration */
        U = B;
    }

    /* returning upper-triangular matrix from PA = LU */
    return U;
}

#ifdef __cplusplus
}
#endif

#endif /* ALGEBRA_H_ */
