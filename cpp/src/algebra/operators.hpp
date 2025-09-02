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

#include "matrix.hpp"

/******************************************************************************/
/*    API                                                                     */
/******************************************************************************/

// When removing const, googletest complains
bool operator==(const Matrix& A, const Matrix& B);

Matrix* operator+(Matrix& A, Matrix& B);

// same case as with + operator
Matrix* operator-(Matrix& A, Matrix &B);

// same case as with + operator
Matrix* operator*(Matrix &A, Matrix &B);

// implicit conversion from ints to floats
Matrix* operator*(const float a, Matrix &B);

Matrix* operator*(Matrix &A, const float b);

Matrix* operator*(Matrix &A, const float b);

// Logically speaking, this method, echelon(), is built on top of the operators
// but it is declared in the Matrix class.
// How to organized code better? or should I wait again until it get more
// structure?
Matrix* Matrix::echelon()
{
    if (this->rows > this->cols)
    {
        LOG_WARNING(this->logMatrix, "The matrix is an overdetermiend system.");
        LOG_WARNING(this->logMatrix, "There are more rows than unknows (rows = ", this->rows, ", cols = ", this->cols, ").");

        return nullptr;
    }

    // rowsPermutation()

    // rowReduction()
    Matrix R;
    R.id(this->rows);
    for (uint32_t i = 0U; i < this->rows; i++)
    {
        auto a = this->val.cbegin() + this->cols * i;
        auto r = R.val.begin() + R.cols * i;
        *r = -1.0F * *a;
    }
    LOG_MATRIX(R);
}

#endif /* OPERATORS_H_ */
