/*******************************************************************************
*
* Matrix Algebra
*
*   SUMMARY
*
*******************************************************************************/

#ifndef ECHELON_H_
#define ECHELON_H_

#include "levels.h"
#include "memory.h"
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <float.h>
#include <math.h>

#include "operators.h"

/******************************************************************************/
/*    API                                                                     */
/******************************************************************************/

/**
 * @brief   Function that computes the echelon form of a square matrix.
 */
MATRIX* echelon(MATRIX *A);

/******************************************************************************/
/*    STATIC FUNCTION PROTOTYPES                                              */
/******************************************************************************/

static uint32_t get_row(MATRIX *A, uint32_t j);

static MATRIX* update(MATRIX *A, uint32_t j);

/******************************************************************************/
/*    IMPLEMENTATION                                                          */
/******************************************************************************/

MATRIX* echelon(MATRIX *A)
{
    if (A->cols != A->rows)
    {
        LOG_WARNING("Matrix is not a square matrix.");
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
            LOG_WARNING("The matrix A is singular.");
            return A;
        }

        for (uint32_t i = j + 1U; i < A->rows; i++)
        {
            /* Let us place the right coef under the pivot (a[i,i]) */
            float a = A->val[A->cols * i + j];

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

    /* returning upper-triangular matrix from PA = LU */
    return A;
}

static uint32_t get_row(MATRIX *A, uint32_t col)
{
    for (uint32_t row = col + 1U; row < A->rows; row++)
    {
        uint32_t idx = TO_C_CONT(A, row, col);
        if (FLT_EPSILON < fabs(A->val[idx]))
        {
            LOG_DEBUG("The pivot is A[%u, %u] = %.8f, row %u.", col, row, A->val[idx], row);
            return row;
        }
    }

    /* returning first index that is out of range (+1) */
    LOG_DEBUG("The column %u has no pivot.", col);
    return A->rows;
}

static MATRIX* update(MATRIX *A, uint32_t col)
{
    uint32_t idx = TO_C_CONT(A, col, col);
    float *pivot = &A->val[idx];
    uint32_t newRow = 0U;
    MATRIX *P = NULL;

    /* no-need to update A, pivot is non-zero */
    if (fabs(pivot[0]) > FLT_EPSILON)
    {
        LOG_DEBUG("The pivot is A[%u, %u] = %.8f, row %u.", col, col, A->val[idx], col);
        return A;
    }

    /* get row of next non-zero value */
    newRow = get_row(A, col);
    if (newRow < A->rows)
    {
        MATRIX vec = {1U, A->cols, NULL};
        MATRIX *row = &vec;
        /* Build Identity and swap rows */
        P = id(A->rows);
        P = permute(P, col, newRow);

        row->val = &A->val[TO_C_CONT(A, col, 0U)];
        LOG_INFO_MATRIX(row);
        row->val = &A->val[TO_C_CONT(A, newRow, 0U)];
        LOG_INFO_MATRIX(row);

        /* Row-permutation, left-multiplication, on A */
        A = mult(P, A);
        LOG_INFO_MATRIX(A);
    }

    return A;
}

#ifdef __cplusplus
}
#endif

#endif /* ECHELON_H_ */
