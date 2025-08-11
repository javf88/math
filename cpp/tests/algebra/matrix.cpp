/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <gtest/gtest.h>
/* TARGET LIBRARY */
#include "matrix.hpp"
#include "levels.hpp"

/******************************************************************************/
/*    TEST FUNCTIONS                                                          */
/******************************************************************************/

TEST(Matrix, Constructors)
{
    using namespace std;
    Log LogUnitTest;

    Matrix Empty;
    ASSERT_EQ(0, Empty.rows);
    ASSERT_EQ(0, Empty.cols);
    ASSERT_EQ(true, Empty.val.empty());
    LOG_MATRIX(LogUnitTest, Empty);

    Matrix A(3,5);
    ASSERT_EQ(3, A.rows);
    ASSERT_EQ(5, A.cols);
    ASSERT_EQ(A.rows * A.cols, A.val.capacity());
    LOG_MATRIX(LogUnitTest, A);

    Matrix B({1, 2, 3, 4, 5, 6, 7});
    ASSERT_EQ(1, B.rows);
    ASSERT_EQ(7, B.cols);
    ASSERT_EQ(B.rows * B.cols, B.val.capacity());
    LOG_MATRIX(LogUnitTest, B);
}

TEST(Matrix, reshape)
{
    using namespace std;

    Matrix A({0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3});

    A.reshape(2,3);
    ASSERT_EQ(1, A.rows);
    ASSERT_EQ(12, A.cols);
    cout << string(A);

    A.reshape(4,3);
    ASSERT_EQ(4, A.rows);
    ASSERT_EQ(3, A.cols);
    cout << string(A);

    A.reshape(4,7);
    ASSERT_EQ(4, A.rows);
    ASSERT_EQ(7, A.cols);
    cout << string(A);
}

TEST(Matrix, transpose)
{
    using namespace std;

    Matrix A({1, 2, 3, 4, 5, 6, 7});
    ASSERT_EQ(1, A.rows);
    ASSERT_EQ(7, A.cols);
    cout << string(A);
    A.transpose();
    ASSERT_EQ(7, A.rows);
    ASSERT_EQ(1, A.cols);
    cout << string(A);

    Matrix B({8, 7, 6, 5, 4, 3, 2, 1});
    B.reshape(8, 3);
    ASSERT_EQ(8, B.rows);
    ASSERT_EQ(3, B.cols);
    cout << string(B);
    B.transpose();
    ASSERT_EQ(3, B.rows);
    ASSERT_EQ(8, B.cols);
    cout << string(B);
}

TEST(Matrix, id)
{
    using namespace std;

    Matrix A;
    A.id(5);
    ASSERT_EQ(5, A.rows);
    ASSERT_EQ(5, A.cols);
    cout << string(A);

    Matrix B({1,2,3,4,5,6});
    B.id(3);
    ASSERT_EQ(3, B.rows);
    ASSERT_EQ(3, B.cols);
    cout << string(B);
}

TEST(Matrix, new)
{
    using namespace std;

    std::stack<void*> *pStack = Static::getStack();
    ASSERT_EQ(0U, pStack->size());

    Matrix *A = new Matrix(0U, 0U);
    ASSERT_EQ(0U, A->rows);
    ASSERT_EQ(0U, A->cols);
    ASSERT_EQ(0U, A->val.capacity());

    Matrix *B = new Matrix(1U, 0U);
    ASSERT_EQ(1U, B->rows);
    ASSERT_EQ(0U, B->cols);
    ASSERT_EQ(0U, B->val.capacity());

    Matrix *C = new Matrix(0U, 1U);
    ASSERT_EQ(0U, C->rows);
    ASSERT_EQ(1U, C->cols);
    ASSERT_EQ(0U, C->val.capacity());

    Matrix *D = new Matrix(3U, 7U);
    ASSERT_EQ(3U, D->rows);
    ASSERT_EQ(7U, D->cols);
    ASSERT_EQ(21U, D->val.capacity());

    ASSERT_EQ(4U, pStack->size());
    Static::clean();
    ASSERT_EQ(0U, pStack->size());
}

TEST(Matrix, log)
{
    using namespace std;

    Matrix A({0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3});
    ASSERT_EQ(1U, A.rows);
    ASSERT_EQ(12U, A.cols);
    // Just displaying [a(0,0), a(0,1), ..., a(0,i), ..., a(0,n-1)]
    Log *tmp = A.log(A.val.cbegin());
    std::cout << tmp->str();

    A.log("A");
    ASSERT_EQ("A", A.name);
    A.log("AB");
    ASSERT_EQ("AB", A.name);
    A.log("Empty");
    ASSERT_EQ("Empty", A.name);

    A.transpose();
    ASSERT_EQ(12U, A.rows);
    ASSERT_EQ(1U, A.cols);

    Matrix B({1,1,1,2,3,4});
    B.reshape(2U, 3U);
    ASSERT_EQ(2U, B.rows);
    ASSERT_EQ(3U, B.cols);
    B.log("B");
    ASSERT_EQ("B", B.name);
    B.log(B.val.cbegin() + B.cols);
    B.log("C");
}
