/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include "levels.hpp"
#include "matrix.hpp"

/******************************************************************************/
/*    IMPLEMENTATION                                                          */
/******************************************************************************/

// When removing const, googletest complains
bool operator==(const Matrix& A, const Matrix& B)
{
    bool ret = true;
    // local logging is required because of the const qualifiers
    // To remove this when logging into file/screen
    Log local;

    if ((A.rows != B.rows) || (A.cols != B.cols))
    {
        LOG_WARNING(local, "Comparison not possible, dimensions do not match.");
        LOG_WARNING(local, "Comparison between ", A.name, " and ", B.name, " not possible.");

        ret = false;
    }
    else
    {
        LOG_INFO(local, "Comparing matrices.");
        for (auto a = A.val.cbegin(), b = B.val.cbegin(); a != A.val.cend(); a++, b++)
        {
            if (*a != *b)
            {
                ret = false;
                break;
            }
        }
    }

    // stream selection here.
    std::cout << local.str();
    return ret;
}

Matrix* operator+(Matrix& A, Matrix& B)
{
    Matrix *C = nullptr;

    if ((A.rows != B.rows) || (A.cols != B.cols))
    {
        LOG_WARNING(A.logMatrix, "Matrices A and B cannot be added.");
        LOG_WARNING(A.logMatrix, "Matrix A is in [", A.rows, "x", A.cols, "].");
        LOG_WARNING(B.logMatrix, "Matrices A and B cannot be added.");
        LOG_WARNING(B.logMatrix, "Matrix B is in [", B.rows, "x", B.cols, "].");
    }
    else
    {
        LOG_WARNING(A.logMatrix, "Adding matrices.");
        LOG_WARNING(B.logMatrix, "Adding matrices.");
        C = new Matrix(A.rows, A.cols);

        for (uint32_t i = 0U; i < A.rows; i++)
        {
            for (uint32_t j = 0U; j < A.cols; j++)
            {
                uint32_t pos = C->cols * i + j;
                C->val[pos] = A.val[pos] + B.val[pos];
            }
        }

        LOG_MATRIX(*C);
    }

    return C;
}

// same case as with + operator
Matrix* operator-(Matrix& A, Matrix &B)
{
    Matrix *C = nullptr;

    if ((A.rows != B.rows) || (A.cols != B.cols))
    {
        LOG_WARNING(A.logMatrix, "Matrices A and B cannot be substracted.");
        LOG_WARNING(A.logMatrix, "Matrix A is in [", A.rows, "x", A.cols, "].");
        LOG_WARNING(B.logMatrix, "Matrices A and B cannot be substracted.");
        LOG_WARNING(B.logMatrix, "Matrix B is in [", B.rows, "x", B.cols, "].");
    }
    else
    {
        LOG_WARNING(A.logMatrix, "Substracting matrices.");
        LOG_WARNING(B.logMatrix, "Substracting matrices.");
        C = new Matrix(A.rows, A.cols);

        for (uint32_t i = 0U; i < A.rows; i++)
        {
            for (uint32_t j = 0U; j < A.cols; j++)
            {
                uint32_t pos = C->cols * i + j;
                C->val[pos] = A.val[pos] - B.val[pos];
            }
        }

        LOG_MATRIX(*C);
    }

    return C;
}

// same case as with + operator
Matrix* operator*(Matrix &A, Matrix &B)
{
    Matrix *C = nullptr;

    if (A.cols != B.rows)
    {
        LOG_WARNING(A.logMatrix, "Matrices A and B cannot be multiply.");
        LOG_WARNING(A.logMatrix, "Matrix A is in [", A.rows, "x", A.cols, "].");
        LOG_WARNING(B.logMatrix, "Matrices A and B cannot be multiply.");
        LOG_WARNING(B.logMatrix, "Matrix B is in [", B.rows, "x", B.cols, "].");
    }
    else
    {
        LOG_WARNING(A.logMatrix, "Multiplying matrices.");
        LOG_WARNING(B.logMatrix, "Multiplying matrices.");
        // C = new Matrix(A.rows, B.cols) behaves oddly when mixing iterators
        // the iterator does point to the memory, but the vector never grows
        // size remains zero
        C = new Matrix;
        C->reshape(A.rows, B.cols);

        for (uint32_t row = 0; row < A.rows; row++)
        {
            auto pRow = A.val.cbegin() + A.cols * row;

            for (uint32_t col = 0U; col < B.cols; col++)
            {
                auto pCol = B.val.cbegin() + col;

                auto pC = C->val.begin() + (C->cols * row) + col;
                *pC = 0.0F;
                LOG_DEBUG(C->logMatrix, "C[", row, ",", col, "] = ", *pC);
                for (uint32_t k = 0U; k < A.cols; k++)
                {
                    *pC += pRow[k] * pCol[B.cols * k];
                    LOG_TRACE(C->logMatrix, "C[", row, ",", col, "] += ",
                                            "A[", A.cols * row, ",", k, "] * ",
                                            "B[", B.cols * k, ",", col, "] = ",
                                            pRow[k] * pCol[B.cols * k]);
                }
                LOG_DEBUG(C->logMatrix, "C[", row, ",", col, "] = ", *pC);
            }
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
