/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include "operators.hpp"
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
    Log local;

    if ((A.rows != B.rows) || (A.cols != B.cols))
    {
        LOG_WARNING(local, "Comparison not possible, dimensions do not match.");
        LOG_WARNING(local, "Comparison between ", A.name, " and ", B.name, " not possible.");

        ret = false;
    }
    else
    {
        for (auto a = A.val.cbegin(), b = B.val.cbegin(); a != A.val.cend(); a++, b++)
        {
            if (*a != *b)
            {
                ret = false;
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
        LOG_WARNING(A.logMatrix, "Matrices A[", A.rows, ",", A.cols, "] and B[", B.rows, ",", B.cols, "] cannot be added.");
        LOG_WARNING(A.logMatrix, "Matrix A is in ", A.rows, "x", A.cols);
        LOG_WARNING(B.logMatrix, "Matrices A[", A.rows, ",", A.cols, "] and B[", B.rows, ",", B.cols, "] cannot be added.");
        LOG_WARNING(B.logMatrix, "Matrix B is in ", B.rows, "x", B.cols);
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
        LOG_WARNING(A.logMatrix, "Matrices A[", A.rows, ",", A.cols, "] and B[", B.rows, ",", B.cols, "] cannot be substracted.");
        LOG_WARNING(A.logMatrix, "Matrix A is in ", A.rows, "x", A.cols);
        LOG_WARNING(B.logMatrix, "Matrices A[", A.rows, ",", A.cols, "] and B[", B.rows, ",", B.cols, "] cannot be substracted.");
        LOG_WARNING(B.logMatrix, "Matrix B is in ", B.rows, "x", B.cols);
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
        LOG_WARNING(A.logMatrix, "Matrices A[", A.rows, ",", A.cols, "] and B[", B.rows, ",", B.cols, "] cannot be multiplied.");
        LOG_WARNING(A.logMatrix, "Matrix A is in ", A.rows, "x", A.cols);
        LOG_WARNING(B.logMatrix, "Matrices A[", A.rows, ",", A.cols, "] and B[", B.rows, ",", B.cols, "] cannot be multiplied.");
        LOG_WARNING(B.logMatrix, "Matrix B is in ", B.rows, "x", B.cols);
    }
    else
    {
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

// Logically speaking, this method, echelon(), is built on top of the operators
// but it is declared in the Matrix class.
// How to organized code better? or should I wait again until it get more
// structure?
Matrix* Matrix::echelon()
{
    // overdertemined case
    if (this->rows > this->cols)
    {
        LOG_WARNING(this->logMatrix, "The matrix is an overdetermiend system.");
        LOG_WARNING(this->logMatrix, "There are more rows than unknows (rows = ", this->rows, ", cols = ", this->cols, ").");

        // Returning nullptr to signal wrong input
        return nullptr;
    }
    else
    {
        // scalar case
        if (this->rows == 1U)
        {
            float scalar = this->val[0U];
            if (std::abs(scalar) < 2 * FLT_EPSILON)
            {
                LOG_ERROR(this->logMatrix, "Scalar zero-matrix.");
            }
            else
            {
                LOG_WARNING(this->logMatrix, "Scalar matrix");
            }

            // Returning itself to signal end of recursion
            return this;
        }
        else
        {
            // general case
            // 1) get P for P x A product, if a(0,0) is zero
            Matrix *PA = this->rowPermute();
            if (PA == nullptr)
            {
                return nullptr;
            }
            else
            {
                LOG_MATRIX(*PA);
                // 2) get L(0) for L(0) x PA = L(0) x LU, to remove 0-th column
                Matrix *LiPA = PA->rowReduction();
                LOG_MATRIX(*LiPA);
                delete PA;
                // 3) get LPA(2,2) as subA in LPA = [LPA(1,1) | LPA(1,2)]
                //                                  [LPA(2,1) | LPA(2,2)]
                Matrix *subA = LiPA->getBlock(1U, LiPA->rows, 1U, LiPA->cols);
                LOG_MATRIX(*subA);
                // 4) call recursively
                Matrix *U = subA->echelon();
                LOG_MATRIX(*U);

                // LiPA is U from previous iteration
                LiPA->setBlock(U);
                return LiPA;
            }
        }
    }
}
