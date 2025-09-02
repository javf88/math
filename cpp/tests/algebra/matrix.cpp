/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <gtest/gtest.h>
/* TARGET LIBRARY */
#include "matrix.hpp"

/******************************************************************************/
/*    TEST FUNCTIONS                                                          */
/******************************************************************************/

TEST(Matrix, Constructors)
{
    Matrix Empty;
    ASSERT_EQ(0, Empty.rows);
    ASSERT_EQ(0, Empty.cols);
    ASSERT_EQ(true, Empty.val.empty());
    LOG_MATRIX(Empty);

    Matrix A(3,5);
    ASSERT_EQ(3, A.rows);
    ASSERT_EQ(5, A.cols);
    ASSERT_EQ(A.rows * A.cols, A.val.capacity());
    LOG_MATRIX(A);

    Matrix B({1, 2, 3, 4, 5, 6, 7});
    ASSERT_EQ(1, B.rows);
    ASSERT_EQ(7, B.cols);
    ASSERT_EQ(B.rows * B.cols, B.val.capacity());
    LOG_MATRIX(B);

    Matrix copy(B);
    ASSERT_EQ(B.rows, copy.rows);
    ASSERT_EQ(B.cols, copy.cols);
    ASSERT_EQ(B.val, copy.val);
    LOG_MATRIX(copy);
}

TEST(Matrix, reshape)
{
    Matrix A({0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3});

    A.reshape(2,3);
    ASSERT_EQ(1, A.rows);
    ASSERT_EQ(12, A.cols);
    LOG_MATRIX(A);

    A.reshape(4,3);
    ASSERT_EQ(4, A.rows);
    ASSERT_EQ(3, A.cols);
    LOG_MATRIX(A);

    A.reshape(4,7);
    ASSERT_EQ(4, A.rows);
    ASSERT_EQ(7, A.cols);
    LOG_MATRIX(A);
}

TEST(Matrix, transpose)
{
    Matrix A({1, 2, 3, 4, 5, 6, 7});
    ASSERT_EQ(1U, A.rows);
    ASSERT_EQ(7U, A.cols);
    LOG_MATRIX(A);
    A.transpose();
    ASSERT_EQ(7U, A.rows);
    ASSERT_EQ(1U, A.cols);
    LOG_MATRIX(A);

    Matrix B({8, 7, 6, 5, 4, 3, 2, 1});
    LOG_MATRIX(B);
    B.reshape(8U, 3U);
    ASSERT_EQ(8U, B.rows);
    ASSERT_EQ(3U, B.cols);
    LOG_MATRIX(B);
    B.transpose();
    ASSERT_EQ(3U, B.rows);
    ASSERT_EQ(8U, B.cols);
    LOG_MATRIX(B);

    Matrix C({1,1,1,2,3,4,2,2,2,5,6,7});
    LOG_MATRIX(C);
    C.reshape(4U, 3U);
    ASSERT_EQ(4U, C.rows);
    ASSERT_EQ(3U, C.cols);
    LOG_MATRIX(C);
    C.transpose();
    ASSERT_EQ(3U, C.rows);
    ASSERT_EQ(4U, C.cols);
    LOG_MATRIX(C);
}

TEST(Matrix, id)
{
    Matrix A;
    LOG_MATRIX(A);
    A.id(5);
    ASSERT_EQ(5, A.rows);
    ASSERT_EQ(5, A.cols);
    LOG_MATRIX(A);

    Matrix B({1,2,3,4,5,6});
    LOG_MATRIX(B);
    B.id(3);
    ASSERT_EQ(3, B.rows);
    ASSERT_EQ(3, B.cols);
    LOG_MATRIX(B);
}

TEST(Matrix, getBlock)
{
    Matrix A({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
    LOG_MATRIX(A);
    A.reshape(4U, 3U);
    LOG_MATRIX(A);

    Matrix *invalidRow = A.getBlock(0U, 0U, 0U, 2U);
    ASSERT_EQ(invalidRow, nullptr);

    Matrix *invalidRow2 = A.getBlock(0U, A.rows + 1U, 0U, 2U);
    ASSERT_EQ(invalidRow2, nullptr);

    Matrix *invalidCol2 = A.getBlock(0U, 2U, 0U, A.cols + 1U);
    ASSERT_EQ(invalidCol2, nullptr);

    Matrix *AA = A.getBlock(0U, A.rows, 0U, A.cols);
    ASSERT_NE(AA, nullptr);
    ASSERT_EQ(A.val, AA->val);
    delete AA;

    Matrix expCol({2, 5, 8, 11});
    expCol.transpose();
    Matrix *aCol = GET_COLUMN_VECTOR(A, A.cols - 1U);
    ASSERT_NE(aCol, nullptr);
    ASSERT_EQ(expCol.val, aCol->val);
    delete aCol;

    Matrix expRow({9, 10, 11});
    Matrix *aRow = GET_ROW_VECTOR(A, A.rows - 1U);
    ASSERT_NE(aRow, nullptr);
    ASSERT_EQ(expRow.val, aRow->val);
    delete aRow;

    Matrix expa22({4, 7});
    expa22.transpose();
    Matrix *a22 = A.getBlock(1U, A.rows - 1U, 1U, A.cols - 1U);
    ASSERT_NE(a22, nullptr);
    ASSERT_EQ(expa22.val, a22->val);
    delete a22;

    Matrix expA11({0});
    Matrix *A11 = A.getBlock(0U, 1U, 0U, 1U);
    ASSERT_NE(A11, nullptr);
    ASSERT_EQ(expA11.val, A11->val);
    delete A11;

    Matrix expA12({1, 2});
    Matrix *A12 = A.getBlock(0U, 1U, 1U, A.cols);
    ASSERT_NE(A12, nullptr);
    ASSERT_EQ(expA12.val, A12->val);
    delete A12;

    Matrix expA21({3, 6, 9});
    expA21.transpose();
    Matrix *A21 = A.getBlock(1U, A.rows, 0U, 1U);
    ASSERT_NE(A21, nullptr);
    ASSERT_EQ(expA21.val, A21->val);
    delete A21;

    Matrix expA22({4, 5, 7, 8, 10, 11});
    expA22.reshape(3U, 2U);
    Matrix *A22 = A.getBlock(1U, A.rows, 1U, A.cols);
    ASSERT_NE(A22, nullptr);
    ASSERT_EQ(expA22.val, A22->val);
    delete A22;
}

TEST(Matrix, newAndDelete)
{
    Matrix *A = new Matrix(0U, 0U);
    ASSERT_EQ(0U, A->rows);
    ASSERT_EQ(0U, A->cols);
    ASSERT_EQ(0U, A->val.capacity());
    LOG_MATRIX(*A);
    delete A;

    Matrix *B = new Matrix(1U, 0U);
    ASSERT_EQ(1U, B->rows);
    ASSERT_EQ(0U, B->cols);
    ASSERT_EQ(0U, B->val.capacity());
    LOG_MATRIX(*B);
    delete B;

    Matrix *C = new Matrix(0U, 1U);
    ASSERT_EQ(0U, C->rows);
    ASSERT_EQ(1U, C->cols);
    ASSERT_EQ(0U, C->val.capacity());
    LOG_MATRIX(*C);
    delete C;

    Matrix *D = new Matrix(3U, 7U);
    ASSERT_EQ(3U, D->rows);
    ASSERT_EQ(7U, D->cols);
    ASSERT_EQ(21U, D->val.capacity());
    LOG_MATRIX(*D);
    delete D;
}

TEST(Matrix, log)
{
    Matrix A({0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3});
    ASSERT_EQ(1U, A.rows);
    ASSERT_EQ(12U, A.cols);
    // Just displaying a(0,0), a(0,1), ..., a(0,i), ..., a(0,n-1)
    Log *tmp = A.log(A.val.cbegin());
    A.logMatrix << tmp->str() << std::endl;;
    delete tmp;

    A.log("A");
    ASSERT_EQ("A", A.name);
    A.log("AB");
    ASSERT_EQ("AB", A.name);
    A.log("Empty");
    ASSERT_EQ("Empty", A.name);

    A.transpose();
    ASSERT_EQ(12U, A.rows);
    ASSERT_EQ(1U, A.cols);
    A.log("A'");

    Matrix B({1,1,1,2,3,4});
    B.log("B");
    B.reshape(2U, 3U);
    ASSERT_EQ(2U, B.rows);
    ASSERT_EQ(3U, B.cols);
    B.log("B");
    ASSERT_EQ("B", B.name);
    // Just displaying [a(0,0), a(0,1), ..., a(0,i), ..., a(0,n-1)]
    tmp = B.log(B.val.cbegin() + B.cols);
    B.logMatrix << tmp->str() << std::endl;;
    delete tmp;

    B.transpose();
    B.log("C");
}

TEST(Matrix, rowPermute)
{
    Matrix A({1,2,3,4,5,0,2,3,4,5,0,3,3,4,5,0,4,4,4,5,5,5,5,5,5});
    A.reshape(5U, 5U);
    LOG_MATRIX(A);
    ASSERT_EQ(&A, A.rowPermute());
}
