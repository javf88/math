/*******************************************************************************
*
* Matrix Algebra
*
*   SUMMARY
*       This single-header module implements a recursive version of
*       the echelon form of a aquare matrix,A in PA = LU, and it returns
*       U after completion.
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

/**
 * @brief   Function that computes the echelon form of a square matrix.
 */
MATRIX* echelon(MATRIX *A);

/******************************************************************************/
/*    STATIC FUNCTION PROTOTYPES                                              */
/******************************************************************************/

static MATRIX* get_lower_triangular(MATRIX *PA);

static MATRIX* get_permutation(MATRIX *A);

/******************************************************************************/
/*    IMPLEMENTATION                                                          */
/******************************************************************************/

MATRIX* echelon(MATRIX *A)
{
    MATRIX *P, *L, *U;
    if (A->cols != A->rows)
    {
        LOG_WARNING("Matrix is not a square matrix.");
        LOG_WARNING_MATRIX(A);
        return NULL;
    }

    if (A->rows == 1U)
    {
        if (fabs(A->val[0U]) < FLT_EPSILON)
        {
            LOG_WARNING("Matrix is singular");
            LOG_WARNING_MATRIX(A);
        }
        else
        {
            LOG_INFO("Matrix is invertible");
            P = L = id(1U);
            U = A;

            LOG_DEBUG("P = L = id(1U)");
            LOG_DEBUG_MATRIX(L);

            LOG_DEBUG("A = U");
            LOG_DEBUG_MATRIX(U);
        }

        return A;
    }

    LOG_DEBUG_MATRIX(A);
    /* 1) get P for P x A product, if a(0,0) is zero */
    MATRIX *PA;
    P = get_permutation(A);
    if (P != NULL)
    {
        LOG_DEBUG_MATRIX(P);
        PA = mult(P, A);
    }
    else
    {
        PA = A;
    }
    LOG_DEBUG_MATRIX(PA);

    /* 2) get L(j) for L(j) x PA = L(j) x LU, to remove j-th column */
    L = get_lower_triangular(PA);
    LOG_DEBUG_MATRIX(L);
    MATRIX *LPA = mult(L, PA);
    LOG_DEBUG_MATRIX(LPA);

    /* 3) get LPA(2,2) as subA in LPA = [LPA(1,1) | LPA(1,2)]
     *                                  [LPA(2,1) | LPA(2,2)] */
    MATRIX *subA = GET_BLOCK_MATRIX(LPA, 1U);
    LOG_DEBUG_MATRIX(subA);

    /* 4) call recursively */
    U = echelon(subA);
    LOG_INFO("return U");
    LOG_DEBUG_MATRIX(U);

    /* 5) set U as LPA(2,2) in LPA */
    U = set_block_matrix(LPA, 1U, 1U, U);
    LOG_DEBUG_MATRIX(U);
    LOG_DEBUG_MATRIX(A);
    LOG_DEBUG_MATRIX(L);

    /* returning upper-triangular matrix from PA = LU */
    return U;
}

uint32_t get_new_pivot(MATRIX *A)
{
    uint32_t row = A->rows;

    for (uint32_t i = 0U; i < A->rows; i++)
    {
        uint32_t pos = TO_C_CONT(A, i, 0U);
        if (fabs(A->val[pos]) > FLT_EPSILON)
        {
            row = i;
            break;
        }
    }

    LOG_TRACE("Pivot %f, A[%u, 0], is in row %u of %u rows", A->val[TO_C_CONT(A, row, 0U)], row, row, A->rows - 1U);
    return row;
}

static MATRIX* get_permutation(MATRIX *A)
{
    MATRIX *P = NULL;

    uint32_t row = get_new_pivot(A);
    if (row == A->rows)
    {
        LOG_WARNING("Matrix is singular.");
    }
    else
    {
        if (row == 0U)
        {
            LOG_DEBUG("Pivot is nonzero, nothing to permute");
        }
        else /* The matrix is nonsingular, pivot is zero and nonscalar */
        {
            P = id(A->rows);
            P = PERMUTE_ROWS(P, row);
        }
    }

    return P;
}

static MATRIX* get_lower_triangular(MATRIX *PA)
{
    MATRIX *L = id(PA->rows);
    MATRIX *a = get_block_matrix(PA, 0U, 1U, 0U, 1U);
    /* when being a scalar it migh be removed by outter for */
    MATRIX *b = get_block_matrix(PA, 1U, PA->rows, 0U, 1U);

    LOG_DEBUG_MATRIX(a);
    a->val[0U] = -1.0F / a->val[0U];
    LOG_DEBUG("1/a is %f", a->val[0U]);
    LOG_DEBUG_MATRIX(b);

    /* building column-vector as B * A^(-1) */
    b = mult(b, a);
    LOG_DEBUG_MATRIX(b);
    L = set_block_matrix(L, 1U, 0U, b);

    return L;
}

MATRIX* get_inverse_lower_triag(MATRIX *L)
{
    /* Copying the matrix */
    MATRIX *invL = GET_BLOCK_MATRIX(L, 0U);

    for (uint32_t i = 1U; i < L->rows; i++)
    {
        uint32_t pos = TO_C_CONT(invL, i, 0U);
        invL->val[pos] = -1.0F * invL->val[pos];
    }

    return invL;
}

#ifdef __cplusplus
}
#endif

#endif /* ECHELON_H_ */
