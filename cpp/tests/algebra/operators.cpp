/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <gtest/gtest.h>
/* TARGET LIBRARY */
#include "operators.hpp"
#include "matrix.hpp"

/******************************************************************************/
/*    TEST CASES                                                              */
/******************************************************************************/

TEST(operators, equality)
{
    Matrix A;
    Matrix B;
    A.id(2U);
    LOG_MATRIX(A);
    B.id(3U);
    LOG_MATRIX(B);
    ASSERT_FALSE(A == B);

    A.id(3);
    LOG_MATRIX(A);
    ASSERT_EQ(A, B);
}

TEST(operators, add)
{
    Matrix *A = new Matrix;
    LOG_MATRIX(*A);
    ASSERT_EQ(0U, A->rows);
    ASSERT_EQ(0U, A->cols);
    ASSERT_EQ(0U, A->val.capacity());

    Matrix *B = new Matrix({1,2,3,4,5,6,7,8});
    LOG_MATRIX(*B);
    ASSERT_EQ(1U, B->rows);
    ASSERT_EQ(8U, B->cols);
    ASSERT_EQ(8U, B->val.size());

    Matrix *C = *A + *B;
    ASSERT_EQ(nullptr, C);
    delete A;

    B->reshape(2U, 4U);
    LOG_MATRIX(*B);
    C = *B + *B;
    delete B;
    ASSERT_EQ(2U, C->rows);
    ASSERT_EQ(4U, C->cols);

    Matrix D({2,4,6,8,10,12,14,16});
    LOG_MATRIX(D);
    D.reshape(2U, 4U);
    LOG_MATRIX(D);
    ASSERT_EQ(D, *C);
    delete C;
}

TEST(operators, substract)
{
    Matrix A;
    LOG_MATRIX(A);
    ASSERT_EQ(0U, A.rows);
    ASSERT_EQ(0U, A.cols);
    ASSERT_EQ(0U, A.val.capacity());

    Matrix B({1,2,3,4,5,6,7,8});
    LOG_MATRIX(B);
    ASSERT_EQ(1U, B.rows);
    ASSERT_EQ(8U, B.cols);
    ASSERT_EQ(8U, B.val.size());

    Matrix *C = A - B;
    ASSERT_EQ(nullptr, C);

    B.reshape(2U, 4U);
    LOG_MATRIX(B);
    C = B - B;
    ASSERT_EQ(2U, C->rows);
    ASSERT_EQ(4U, C->cols);

    Matrix D(2U, 4U);
    LOG_MATRIX(D);
    ASSERT_EQ(D, *C);
    delete C;
}

TEST(operators, multiply)
{
    Matrix A;
    LOG_MATRIX(A);
    ASSERT_EQ(0U, A.rows);
    ASSERT_EQ(0U, A.cols);
    ASSERT_EQ(0U, A.val.capacity());

    Matrix B({1,2,3,4,5,6,7,8,9,10,11,12});
    LOG_MATRIX(B);
    B.reshape(3U, 4U);
    LOG_MATRIX(B);
    ASSERT_EQ(3U, B.rows);
    ASSERT_EQ(4U, B.cols);
    ASSERT_EQ(12U, B.val.size());

    Matrix *C = A * B;
    ASSERT_EQ(nullptr, C);

    Matrix D({3,0,0,0,3,0,0,0,3});
    LOG_MATRIX(D);
    D.reshape(3U, 3U);
    LOG_MATRIX(D);

    Matrix *E = D * B;
    LOG_MATRIX(*E);
    ASSERT_EQ(3U, E->rows);
    ASSERT_EQ(4U, E->cols);
    delete E;
}

TEST(operators, scalar)
{
    Matrix B({1,2,3,4,5,6,7,8});
    LOG_MATRIX(B);
    B.reshape(2U, 4U);
    LOG_MATRIX(B);
    ASSERT_EQ(2U, B.rows);
    ASSERT_EQ(4U, B.cols);

    Matrix *C = 4.0F * B;
    ASSERT_EQ(2U, C->rows);
    ASSERT_EQ(4U, C->cols);
    LOG_MATRIX(*C);

    Matrix D({4,8,12,16,20,24,28,32});
    LOG_MATRIX(D);
    D.reshape(2U, 4U);
    LOG_MATRIX(D);
    ASSERT_EQ(2U, D.rows);
    ASSERT_EQ(4U, D.cols);
    ASSERT_EQ(D, *C);
    delete C;

    Matrix *E = B * 4.0;
    ASSERT_EQ(D, *E);
    LOG_MATRIX(*E);
    delete E;

    Matrix *F = 4 * B;
    ASSERT_EQ(D, *F);
    LOG_MATRIX(*F);
    delete F;

    Matrix *G = B * 4U;
    ASSERT_EQ(D, *G);
    LOG_MATRIX(*G);
    delete G;
}

TEST(operators, echelonEdgeCases)
{
    Matrix A({1,2,3,4,5,6,7,8});
    A.transpose();
    LOG_MATRIX(A);

    // overdertemined case
    Matrix *ret = A.echelon();
    ASSERT_EQ(nullptr, ret);

    // scalar case
    Matrix a({5});
    LOG_MATRIX(a);
    ret = a.echelon();
    ASSERT_EQ(ret, &a);

    // scalar zero case
    Matrix z({0});
    LOG_MATRIX(z);
    ret = z.echelon();
    ASSERT_EQ(ret, &z);

    // 2x2 singular case
    Matrix S({0,0,0,1});
    S.reshape(2U, 2U);
    LOG_MATRIX(S);
    ret = S.echelon();
    ASSERT_EQ(nullptr, ret);
    /*
    // no permutation case
    Matrix B;//({0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0});
    B.id(4U);
    LOG_MATRIX(B);
    ret = B.echelon();
    ASSERT_EQ(nullptr, ret);

    // singular case, no permuatation
    Matrix C({0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0});
    C.reshape(4U, 4U);
    LOG_MATRIX(C);
    ret = C.echelon();
    ASSERT_EQ(nullptr, ret);

    // Permutation case
    Matrix D({0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0});
    D.reshape(4U, 4U);
    LOG_MATRIX(D);
    ret = D.echelon();
    ASSERT_NE(nullptr, ret);
*/
}

TEST(operators, echelon)
{
    // 2x2 case with no permutation
    Matrix B({1,2,3,4});
    B.reshape(2U, 2U);
    LOG_MATRIX(B);
    Matrix *U = B.echelon();
    delete U;
}
