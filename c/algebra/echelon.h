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
    if (A->cols != A->rows)
    {
        LOG_WARNING("Matrix is not a square matrix.");
        return NULL;
    }

    MATRIX *U = A;
    for (uint32_t j = 0U; j < (A->cols - 1U); j++)
    {
        MATRIX *PA, *L, *LPA, *P;
        MATRIX *subA;

        /* 1) get P for P x A product, if a(0,0) is zero */
        P = get_permutation(A);
        if (P != NULL)
        {
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
        LPA = mult(L, PA);
        LOG_DEBUG_MATRIX(LPA);

        /* 3) get LPA(2,2) as subA in LPA = [LPA(1,1) | LPA(1,2)]
         *                                  [LPA(2,1) | LPA(2,2)]*/
        subA = GET_BLOCK_MATRIX(LPA, j + 1U);
        LOG_DEBUG_MATRIX(subA);
        /* 4) call recursively */
        U = echelon(subA);
        LOG_INFO("return U");
        LOG_DEBUG_MATRIX(U);
        if (U->rows == 1U)
        {
            if (fabs(U->val[0U]) < FLT_EPSILON)
            {
                LOG_WARNING("Matrix is singular.");
            }
        }

        /* 5) set U as LPS(2,2) in LPA */
        U = set_block_matrix(LPA, j + 1U, j + 1U, U);
        LOG_DEBUG_MATRIX(U);

        /* 6) break iteration to return back */
        break;
    }

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
        P = id(A->rows);
        /* this if might be removed due to outter for loop */
        if (A->rows > 1U)
        {
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
    LOG_DEBUG_MATRIX(b);

    /* building column-vector as B * A^(-1) */
    b = mult(b, a);
    L = set_block_matrix(L, 1U, 0U, b);

    return L;
}

#ifdef __cplusplus
}
#endif

#endif /* ECHELON_H_ */
