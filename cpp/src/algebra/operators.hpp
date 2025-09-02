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

#endif /* OPERATORS_H_ */
