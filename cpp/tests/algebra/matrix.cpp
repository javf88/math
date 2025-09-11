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

// There is a bug here with the memory management
TEST(Matrix, getBlock)
{
    Matrix A({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
    LOG_MATRIX(A);
    Matrix *invalidRow = A.getBlock();
    ASSERT_EQ(invalidRow, nullptr);

    A.transpose();
    LOG_MATRIX(A);
    Matrix *invalidCol = A.getBlock();
    ASSERT_EQ(invalidCol, nullptr);

    Matrix s({3});
    Matrix B({0, 1, 2, 3});
    B.reshape(2U, 2U);
    LOG_MATRIX(B);
    Matrix *scalar = B.getBlock();
    ASSERT_EQ(scalar->rows, 1U);
    ASSERT_EQ(scalar->cols, 1U);
    ASSERT_EQ(s.val, scalar->val);
    delete scalar;

    Matrix cRow({7,8,9,10,11});
    Matrix C({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
    C.reshape(2U, 6U);
    LOG_MATRIX(C);
    Matrix *row = C.getBlock();
    ASSERT_EQ(row->rows, 1U);
    ASSERT_EQ(row->cols, 5U);
    ASSERT_EQ(cRow.val, row->val);
    delete row;

    Matrix cCol({3,5,7,9,11});
    C.transpose();
    LOG_MATRIX(C);
    Matrix *col = C.getBlock();
    ASSERT_EQ(col->rows, 5U);
    ASSERT_EQ(col->cols, 1U);
    ASSERT_EQ(cCol.val, col->val);
    delete col;

    Matrix d1({5,6,7,9,10,11});
    d1.reshape(2U, 3U);
    Matrix D({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
    D.reshape(3U, 4U);
    LOG_MATRIX(D);
    Matrix *block = D.getBlock();
    ASSERT_EQ(block->rows, 2U);
    ASSERT_EQ(block->cols, 3U);
    ASSERT_EQ(d1.val, block->val);
    delete block;

    Matrix d2({1,5,7,2,10,11});
    d2.reshape(3U, 2U);
    D.transpose();
    LOG_MATRIX(D);
    block = D.getBlock();
    ASSERT_EQ(block->rows, 3U);
    ASSERT_EQ(block->cols, 2U);
    ASSERT_EQ(d2.val, block->val);
    delete block;
}

TEST(Matrix, setBlock)
{
    Matrix A;
    A.id(5U);
    LOG_MATRIX(A);

    Matrix B({1,2,3,4,2,2,2,2,4,3,2,1,4,4,4,4});
    B.reshape(4U, 4U);
    LOG_MATRIX(B);
    A.setBlock(&B);
    LOG_MATRIX(A);

    Matrix R({1,0,0,0,0,0,1,2,3,4,0,2,2,2,2,0,4,3,2,1,0,4,4,4,4});
    ASSERT_EQ(R.val, A.val);
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
    Matrix A({1,2,3,4,5,0,2,2,2,2,0,3,3,3,4,0,4,4,4,4,5,5,5,5,5});
    A.reshape(5U, 5U);
    LOG_MATRIX(A);
    // nothing to permute, A = PA
    ASSERT_EQ(&A, A.rowPermute());

    A.val[0U] = 0.0F;
    LOG_MATRIX(A);
    Matrix *tmp = A.rowPermute();
    // Permutation
    ASSERT_NE(nullptr, tmp);
    ASSERT_NE(&A, tmp);

    A.val[A.rows * 4U] = 0.0F;
    LOG_MATRIX(A);
    // singular matrix
    tmp = A.rowPermute();
    ASSERT_EQ(nullptr, tmp);
    delete tmp;
}

TEST(Matrix, rowReduction)
{
    Matrix A({1,2,3,4,5,1,2,2,2,2,2,3,3,3,3,3,4,4,4,4,4,5,5,5,5});
    A.reshape(5U, 5U);
    LOG_MATRIX(A);
    Matrix *L1PA = A.rowReduction();
    for (uint32_t k = 1U; k < L1PA->rows; k++)
    {
        auto a = L1PA->val.cbegin() + (L1PA->cols * k);
        ASSERT_EQ(0.0F, *a);
    }
    delete L1PA;
}
