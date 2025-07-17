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
    using namespace std;

    Matrix Empty;
    ASSERT_EQ(0, Empty.rows);
    ASSERT_EQ(0, Empty.cols);
    ASSERT_EQ(true, Empty.val.empty());
    cout << string(Empty) << endl;

    Matrix A(3,5);
    ASSERT_EQ(3, A.rows);
    ASSERT_EQ(5, A.cols);
    ASSERT_EQ(A.rows * A.cols, A.val.capacity());
    cout << string(A) << endl;

    Matrix B({1, 2, 3, 4, 5, 6, 7});
    ASSERT_EQ(1, B.rows);
    ASSERT_EQ(7, B.cols);
    ASSERT_EQ(B.rows * B.cols, B.val.capacity());
    cout << string(B) << endl;
}

TEST(Matrix, reshape)
{
    using namespace std;

    Matrix A({0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3});

    A.reshape(2,3);
    ASSERT_EQ(1, A.rows);
    ASSERT_EQ(12, A.cols);
    cout << string(A) << endl;

    A.reshape(4,3);
    ASSERT_EQ(4, A.rows);
    ASSERT_EQ(3, A.cols);
    cout << string(A) << endl;

    A.reshape(4,7);
    ASSERT_EQ(4, A.rows);
    ASSERT_EQ(7, A.cols);
    cout << string(A) << endl;
}

TEST(Matrix, transpose)
{
    using namespace std;

    Matrix A({1, 2, 3, 4, 5, 6, 7});
    ASSERT_EQ(1, A.rows);
    ASSERT_EQ(7, A.cols);
    cout << string(A) << endl;
    A.transpose();
    ASSERT_EQ(7, A.rows);
    ASSERT_EQ(1, A.cols);
    cout << string(A) << endl;

    Matrix B({8, 7, 6, 5, 4, 3, 2, 1});
    B.reshape(8, 3);
    ASSERT_EQ(8, B.rows);
    ASSERT_EQ(3, B.cols);
    cout << string(B) << endl;
    B.transpose();
    ASSERT_EQ(3, B.rows);
    ASSERT_EQ(8, B.cols);
    cout << string(B) << endl;
}

TEST(Matrix, id)
{
    using namespace std;

    Matrix A;
    A.id(5);
    ASSERT_EQ(5, A.rows);
    ASSERT_EQ(5, A.cols);
    cout << string(A) << endl;

    Matrix B({1,2,3,4,5,6});
    B.id(3);
    ASSERT_EQ(3, B.rows);
    ASSERT_EQ(3, B.cols);
    cout << string(B) << endl;
}
