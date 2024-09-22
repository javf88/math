/*******************************************************************************
*
* Matrix Algebra
*
*   SUMMARY
*
*******************************************************************************/

#ifndef ECHELON_H_
#define ECHELON_H_

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

MATRIX* echelon(MATRIX *U);

/******************************************************************************/
/*    STATIC FUNCTION PROTOTYPES                                              */
/******************************************************************************/

static uint32_t get_row(MATRIX *U, uint32_t j);

static MATRIX* update(MATRIX *U, uint32_t j);

/******************************************************************************/
/*    IMPLEMENTATION                                                          */
/******************************************************************************/

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

static uint32_t get_row(MATRIX *U, uint32_t j)
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

static MATRIX* update(MATRIX *U, uint32_t j)
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

#ifdef __cplusplus
}
#endif

#endif /* ECHELON_H_ */
