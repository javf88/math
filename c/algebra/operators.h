/*******************************************************************************
*
* Matrix Algebra - matrix operators
*
*   SUMMARY
*       This is a single-header submodule that implements some basic 
*       algebraic operators for matrices:
*
*       - transpose
*       - addition
*       - subtraction
*       - multiplication
*       - Identity matrix
*       - Row-permutation matrix
*
*******************************************************************************/

#ifndef OPERATORS_H_
#define OPERATORS_H_

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
/*    DEFINITIONS                                                             */
/******************************************************************************/

/* Mapping from a matrix, A[i, j], to C-contiguous layout */
#define TO_C_CONT(A, row, col) A->cols * row + col

/* Mapping from a matrix, AT[i, j], to Fortran-contiguous layout */
#define TO_F_CONT(A, row, col) A->rows * col + row

/******************************************************************************/
/*    API                                                                     */
/******************************************************************************/

/**
 * @brief   Function that transpose a matrix
 *
 * @summary MATRIX's memory layout is c-contiguous. When it is transposed
 *          it yields a MATRIX with the Fortran-contiguous layout.
 */
MATRIX* transpose(MATRIX *A);

/**
 * @brief   Function that adds to matrices of the same size/dimension.
 */
MATRIX* add(MATRIX *A, MATRIX *B);

/**
 * @brief   Function that subtracts to matrices of the same size/dimension.
 */
MATRIX* sub(MATRIX *A, MATRIX *B);

/**
 * @brief   Function that multiplies to matrices if their dimensions
 *          are appropiate.
 */
MATRIX* mult(MATRIX *A, MATRIX *B);

/**
 * @brief   Function that generated an identity matrix of dimension size.
 */
MATRIX* id(uint32_t size);

/**
 * @brief   Function that swap two rows of an identity matrix, to generate
 *          a row-permutation matrix.
 */
MATRIX* permute(MATRIX *I, uint32_t a, uint32_t b);

/******************************************************************************/
/*    IMPLEMENTATION                                                          */
/******************************************************************************/

MATRIX* transpose(MATRIX *A)
{
    MATRIX *AT = NULL;
    if (A == NULL)
    {
        LOG_WARNING("Nothing to transpose: Empty matrix!");
        return (MATRIX*) NULL;
    }

    AT = push_matrix(A->cols, A->rows);
    if (AT != NULL)
    {
        for (uint32_t row = 0U; row < A->rows; row++)
        {
            for (uint32_t col = 0U; col < A->cols; col++)
            {
                uint32_t pos = TO_C_CONT(A, row, col);     //A->cols * row + col;
                uint32_t posT = TO_F_CONT(A, row, col);    //A->rows * col + row;

                AT->val[posT] = A->val[pos];
            }
        }
    }

    return AT;
}

MATRIX* add(MATRIX *A, MATRIX *B)
{
    MATRIX *C = NULL;
    if ((A == NULL) || (B == NULL))
    {
        LOG_ERROR("Wrong inputs in add(A, B).");
        return (MATRIX*)NULL;
    }
    else
    {
        if ((A->rows != B->rows) || (A->cols != B->cols))
        {
            LOG_WARNING("Matrices does not have suitable size for matrix-addition.");
            return (MATRIX*)NULL;
        }
    }

    C = push_matrix(A->rows, A->cols);
    if (C != NULL)
    {
        for (uint32_t row = 0U; row < A->rows; row++)
        {
            for (uint32_t col = 0U; col < A->cols; col++)
            {
                uint32_t pos = TO_C_CONT(A, row, col);
                C->val[pos] = A->val[pos] + B->val[pos];
            }
        }
    }

    return C;
}

MATRIX* sub(MATRIX *A, MATRIX *B)
{
    MATRIX *C = NULL;
    if ((A == NULL) || (B == NULL))
    {
        LOG_ERROR("Wrong inputs in sub(A, B).");
        return (MATRIX*)NULL;
    }
    else
    {
        if ((A->rows != B->rows) || (A->cols != B->cols))
        {
            LOG_ERROR("Matrices does not have suitable size for matrix-subtraction.");
            return (MATRIX*)NULL;
        }
    }

    C = push_matrix(A->rows, A->cols);
    if (C != NULL)
    {
        for (uint32_t row = 0U; row < A->rows; row++)
        {
            for (uint32_t col = 0U; col < A->cols; col++)
            {
                uint32_t pos = TO_C_CONT(A, row, col);
                C->val[pos] = A->val[pos] - B->val[pos];
            }
        }
    }

    return C;
}

MATRIX* mult(MATRIX *A, MATRIX *B)
{
    MATRIX *C = NULL;
    if ((A == NULL) || (B == NULL))
    {
        LOG_ERROR("Wrong inputs in mult(A, B).");
        return (MATRIX*)NULL;
    }
    else
    {
        if (A->cols != B->rows)
        {
            LOG_ERROR("Matrices does not have suitable size for matrix-multiplication.");
            return (MATRIX*)NULL;
        }
    }

    C = push_matrix(A->rows, B->cols);
    if (C != NULL)
    {
        for (uint32_t row = 0U; row < A->rows; row++)
        {
            uint32_t vecA = A->cols * row;

            for (uint32_t col = 0U; col < B->cols; col++)
            {
                uint32_t pos = TO_C_CONT(C, row, col);
                LOG_TRACE("C[%u,%u] ~ C[%u]", row, col, pos);

                for (uint32_t i = 0; i < A->cols; i++)
                {
                    uint32_t vecB = B->cols * i;
                    LOG_TRACE("A[%u,%u] * B[%u,%u] := A[%u] x B[%u]", row, i, col, vecB + col, vecA + i, vecB + col);
                    C->val[pos] += A->val[vecA + i] * B->val[vecB + col];
                }
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

    memset(I->val, 0U, I->rows * I->cols);
    for (uint32_t i = 0U; i < I->rows; i++)
    {
        uint32_t pos = TO_C_CONT(I, i, i);
        I->val[pos] = 1.0F;
    }

    return I;
}

MATRIX* permute(MATRIX *I, uint32_t a, uint32_t b)
{
    if (I == NULL)
    {
        LOG_WARNING("Nothing to swap: Empty matrix!");
        return (MATRIX*) NULL;
    }
    else
    {
        if (I->rows <= a || I->rows <= b)
        {
            LOG_ERROR("Permutation indecis(%u,%u) are out of range 0...%d (Id[%u])", a, b, I->rows - 1U, I->rows);
            return I;
        }
    }

    /* permuting(swapping) I[a,*] with I[b,*] rows */
    float *posA = &I->val[I->cols * a + a];
    float *posB = &I->val[I->cols * b + b];

    float *newA = &I->val[I->cols * a + b];
    float *newB = &I->val[I->cols * b + a];

    posA[0U] = posB[0U] = 0.0F;
    newA[0U] = newB[0U] = 1.0F;

    return I;
}

#ifdef __cplusplus
}
#endif

#endif /* OPERATORS_H_ */
