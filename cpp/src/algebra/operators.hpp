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
    Log LogOperators;
    if ((A.rows != B.rows) || (A.cols != B.cols))
    {
        LOG_WARNING(LogOperators, "Comparison not possible, dimensions do not match.");
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
    Log LogOperators;
    Matrix *C = nullptr;

    if ((A.rows != B.rows) || (A.cols != B.cols))
    {
        LOG_WARNING(LogOperators, "Matrices A[", A.rows, ",", A.cols,
                "] and B[", B.rows, ",", B.cols, "] cannot be added.");
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
    Log LogOperators;
    Matrix *C = nullptr;

    if ((A.rows != B.rows) || (A.cols != B.cols))
    {
        LOG_WARNING(LogOperators, "Matrices A[", A.rows, "x", A.cols,
                "] and B[", B.rows, "x", B.cols, "] cannot be substracted.");
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
    Log LogOperators;
    Matrix *C = nullptr;

    if (A.cols != B.rows)
    {
        LOG_WARNING(LogOperators, "Matrices dimensions do not match [",
                A.rows, "x", A.cols, "] * [", B.rows, "x", B.cols, "].");
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
