#ifndef ALGEBRA_H_
#define ALGEBRA_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <stdio.h>

#include "memory.h"

/******************************************************************************/
/*    TEST FUNCTIONS                                                          */
/******************************************************************************/

MATRIX* transpose(MATRIX *A)
{
    if (A == NULL)
    {
        printf("Nothing to transpose: Empty matrix!");
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
    if ((A->rows != B->rows) || (A->rows != B->rows))
    {
        printf("Matrices does not have suitable size for matrix-addition.\n");
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
    if ((A->rows != B->rows) || (A->rows != B->rows))
    {
        printf("Matrices does not have suitable size for matrix-addition.\n");
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
        printf("Matrices does not have suitable size for matrix-multiplication.\n");
        return (MATRIX*)NULL;
    }

    MATRIX *C = push_matrix(A->rows, B->cols);

    for (uint32_t row = 0U; row < A->rows; row++)
    {
        uint32_t row2pos = A->cols * row;

        for (uint32_t col = 0U; col < B->cols; col++)
        {
            uint32_t pos = row2pos + col;

            for (uint32_t i = 0; i < A->cols; i++)
            {
                C->val[pos] += A->val[row2pos + i] * B->val[B->cols * i + col];
            }
        }
    }

    return C;
}

void _print(MATRIX *A)
{
    if (A == NULL)
    {
        printf("Nothing to print: Matrix is empty!");
    }

    for (uint32_t i = 0U; i < A->rows; i++)
    {
        printf("\t[");
        for (uint32_t j = 0U; j < A->cols; j++)
        {
            printf("%04.4f, ", A->val[i * A->cols + j]);
        }
        printf("]\n");
    }
    printf("\n");
}

#ifdef __cplusplus
}
#endif

#endif /* ALGEBRA_H_ */
