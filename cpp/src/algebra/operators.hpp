/*******************************************************************************
*
* Operators
*
*   SUMMARY
*
*******************************************************************************/

#ifndef OPERATORS_H_
#define OPERATORS_H_

/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <cstdint>
#include <cstdio>

#include "matrix.hpp"

/******************************************************************************/
/*    API                                                                     */
/******************************************************************************/

bool operator==(const Matrix& A, const Matrix& B)
{
    if ((A.rows != B.rows) || (A.cols != B.cols))
    {
        return false;
    }
    else
    {
        for (auto a = A.val.cbegin(), b = B.val.cbegin(); a != A.val.cend(); a++, b++)
        {
            if (*a != *b)
            {
                return false;
            }
        }
    }
    return true;
}

Matrix* operator+(const Matrix& A, const Matrix& B)
{
    Matrix *C = nullptr;

    if ((A.rows != B.rows) || (A.cols != B.cols))
    {
        std::fprintf(stderr, "Matrices A[%hu,%hu] and B[%hu,%hu] cannot be added.\n",
                A.rows, A.cols, B.rows,B.cols);

    }
    else
    {
        C = new Matrix(A.rows, A.cols);

        for (uint32_t i = 0U; i < A.rows; i++)
        {
            for (uint32_t j = 0U; j < A.cols; j++)
            {
                uint32_t pos = C->cols * i + j;
                C->val[pos] = A.val[pos] + B.val[pos];
            }
        }
    }

    return C;
}

Matrix* operator-(const Matrix& A, const Matrix &B)
{
    Matrix *C = nullptr;

    if ((A.rows != B.rows) || (A.cols != B.cols))
    {
        std::fprintf(stderr, "Matrices A[%hu,%hu] and B[%hu,%hu] cannot be substracted.\n",
                A.rows, A.cols, B.rows,B.cols);

    }
    else
    {
        C = new Matrix(A.rows, A.cols);

        for (uint32_t i = 0U; i < A.rows; i++)
        {
            for (uint32_t j = 0U; j < A.cols; j++)
            {
                uint32_t pos = C->cols * i + j;
                C->val[pos] = A.val[pos] - B.val[pos];
            }
        }
    }

    return C;
}

Matrix* operator*(const Matrix &A, const Matrix &B)
{
    Matrix *C = nullptr;

    if (A.cols != B.rows)
    {
        std::printf("Matrices dimensions do not match [%ux%u] * [%ux%u]\n", A.rows, A.cols, B.rows, B.cols);

        return nullptr;
    }
    else
    {
        C = new Matrix(A.rows, B.cols);
        std::cout << std::string(B) << std::endl;
        std::cout << std::string(A) << std::endl;
        std::cout << std::string(*C) << std::endl;

        for (uint32_t row = 0; row < C->rows; row++)
        {
            uint32_t i = C->cols * row;

            for (uint32_t col = 0; col < C->cols; col++)
            {
                uint32_t pos = i + col;
                std::fprintf(stderr, "(i,j) = (%hu,%hu) = %hu\n", i, col, pos);

                C->val[pos] = 0.0F;

                for (uint32_t k = 0; k < A.cols; k++)
                {
                    uint32_t j = B.cols * k + col;

                    std::fprintf(stderr, "[pos] <- [i,k]*[k,j] = %hu <- [%hu,%hu]*[%hu,%hu]\n",
                            pos, i, k, B.cols * k, col);
                    C->val[pos] += A.val[i + k] * B.val[j];
                }
            }

            std::cout << std::string(*C) << std::endl;
        }
    }

    return C;
}

// implicit conversion from ints to floats
Matrix* operator*(const float a, Matrix &B)
{
    Matrix *C = new Matrix;
    C->reshape(B.rows, B.cols);

    for (auto b = B.val.begin(), c = C->val.begin(); b != B.val.end(); b++, c++)
    {
        *c = a * *b;
    }

    return C;
}

Matrix* operator*(Matrix &A, const float b)
{
    Matrix *C = b * A;

    return C;
}
#endif /* OPERATORS_H_ */
