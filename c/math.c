#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Matrix
{
    uint32_t rows;
    uint32_t cols;
    float   *val;
} MATRIX;


MATRIX* _malloc(uint32_t rows, uint32_t cols)
{
    MATRIX *matrix = malloc(sizeof(MATRIX));

    matrix->rows = rows;
    matrix->cols = cols;
    matrix->val = malloc(sizeof(double) * rows * cols);

    return matrix;
}

MATRIX* _free(MATRIX *matrix)
{
    if (matrix->val != NULL)
    {
        free(matrix->val);
    }

    if (matrix != NULL)
    {
        free(matrix);
    }

    return matrix = (MATRIX*)NULL;
}

MATRIX* add(MATRIX *A, MATRIX *B)
{
    if ((A->rows != B->rows) || (A->rows != B->rows))
    {
        printf("Matrices does not have suitable size for matrix-addition.\n");
        return (MATRIX*)NULL;
    }

    MATRIX *C = _malloc(A->rows, A->cols);

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

    MATRIX *C = _malloc(A->rows, A->cols);

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

    MATRIX *C = _malloc(A->rows, B->cols);

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

MATRIX* transpose(MATRIX *A)
{
    if (A == NULL)
    {
        printf("Nothing to transpose: Empty matrix!");
        return (MATRIX*) NULL;
    }

    MATRIX *AT = _malloc(A->cols, A->rows);

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

int main()
{
    printf("Initial commit\n");

    MATRIX *A = _malloc(1U, 3U);
    for (uint32_t i = 0U; i < 3U; i++)
    {
        A->val[i] = (double)i;
    }
    _print(A);

    MATRIX *D = add(A, A);
    _print(D);

    MATRIX *E = subs(D, A);
    _print(E);
    return 0;


    MATRIX *B = _malloc(4U, 7U);
    for (uint32_t i = 0U; i < (4U * 7U); i++)
    {
        B->val[i] = (double)i;
    }
    _print(B);
    MATRIX *BT = transpose(B);
    _print(BT);

    MATRIX *C = mult(A, B);
    MATRIX *AT = transpose(A);
    _print(A);
    _print(AT);

    MATRIX *ATA = mult(A, AT);
    _print(ATA);

    _free(ATA);
    _free(A);
    _free(B);
    if (C != NULL)
    {
        _free(C);
    }

    return 0;
}
