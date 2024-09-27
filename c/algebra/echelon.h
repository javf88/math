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

void* decomposition(MATRIX *A);

/******************************************************************************/
/*    PRIVATE DATA                                                            */
/******************************************************************************/

typedef struct PA_LU
{
    MATRIX *P;
    MATRIX *PT;
    MATRIX *A;
    MATRIX *LI;
    MATRIX *L;
    MATRIX *U;
} PA_LU;

/******************************************************************************/
/*    STATIC FUNCTION PROTOTYPES                                              */
/******************************************************************************/

static MATRIX* get_lower_triangular(MATRIX *PA);

static MATRIX* get_permutation(MATRIX *A);

static MATRIX* inverse(MATRIX *L);

/******************************************************************************/
/*    IMPLEMENTATION                                                          */
/******************************************************************************/

MATRIX* echelon(MATRIX *A)
{
    PA_LU *PA_LU = decomposition(A);
    if (PA_LU != NULL)
    {
        return PA_LU->U;
    }

    return NULL;;
}

void* decomposition(MATRIX *A)
{
    PA_LU *PA_LU = NULL;
    /* Matrices of interes of (PA,LU) pencil  */
    MATRIX *L, *P, *U;
    /* Partial results */
    MATRIX *PA, *LPA, *LI;
    /* subproblem for next iteration */
    MATRIX *A22;

    if (A->cols != A->rows)
    {
        LOG_WARNING("Matrix is not a square matrix.");
        return NULL;
    }

    if (A->rows == 1U)
    {
        if (fabs(A->val[0U]) < FLT_EPSILON)
        {
            LOG_WARNING("Matrix is singular.");
        }
        PA_LU = malloc(sizeof(MATRIX*) * 6U);
        PA_LU->PT = PA_LU->P = PA_LU->LI = PA_LU->L = id(1U);
        PA_LU->A = PA_LU->U = A;
        LOG_DEBUG_MATRIX(A);
        return PA_LU;
    }

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
    /* get the inverse of L */
    LI = inverse(L);
    LOG_DEBUG_MATRIX(L);
    LPA = mult(L, PA);
    LOG_DEBUG_MATRIX(LPA);

    /* 3) get LPA(2,2) as A22 in LPA = [LPA(1,1) | LPA(1,2)]
     *                                  [LPA(2,1) | LPA(2,2)]*/
    A22 = GET_BLOCK_MATRIX(LPA, 1U);
    LOG_DEBUG_MATRIX(A22);
    /* 4) call recursively */
    PA_LU = decomposition(A22);
    LOG_INFO("return U");

    /* 5) set U as LPA(2,2) in LPA */
    U = set_block_matrix(LPA, 1U, 1U, PA_LU->U);
    LOG_DEBUG_MATRIX(U);

    /* returning upper-triangular matrix from PA = LU */
    PA_LU->PT = PA_LU->P = P;
    PA_LU->LI = LI;
    PA_LU->L = L;
    PA_LU->A = A;
    PA_LU->U = U;

    return PA_LU;
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

static MATRIX* inverse(MATRIX *L)
{
    MATRIX *LI = COPY_MATRIX(L);

    for (uint32_t i = 1U; i < LI->rows; i++)
    {
        uint32_t idx = TO_C_CONT(LI, i, 0U);
        LI->val[idx] = -L->val[idx];
    }

    return LI;
}

#ifdef __cplusplus
}
#endif

#endif /* ECHELON_H_ */
