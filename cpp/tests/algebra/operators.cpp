/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <gtest/gtest.h>
/* TARGET LIBRARY */
#include "operators.hpp"
#include "matrix.hpp"

/******************************************************************************/
/*    FIXTURES                                                                */
/******************************************************************************/

class DummyStack: public testing::Test
{
public:
    void SetUp() override
    {
        using namespace std;

        // To improve logging message.
        cout << "Running SetUp()" << endl;

        stack<void*> *pStack = Static::getStack();
        for (uint32_t i = 0U; i < 32U; i++)
        {
            void *pInt = new int(i);
            pStack->push(pInt);
        }
    }
};

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
    ASSERT_EQ(0U, A->rows);
    ASSERT_EQ(0U, A->cols);
    ASSERT_EQ(0U, A->val.capacity());

    Matrix *B = new Matrix({1,2,3,4,5,6,7,8});
    LOG_PMATRIX(*B);
    ASSERT_EQ(1U, B->rows);
    ASSERT_EQ(8U, B->cols);
    ASSERT_EQ(8U, B->val.size());

    Matrix *C = *A + *B;
    ASSERT_EQ(nullptr, C);
    delete A;

    B->reshape(2U, 4U);
    LOG_PMATRIX(*B);
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

    // delete might move to stack
    Static::clean();
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

    Static::clean();
}

/*
TEST(operators, scalar)
{
    Matrix B({1,2,3,4,5,6,7,8});
    B.reshape(2U, 4U);
    ASSERT_EQ(2U, B.rows);
    ASSERT_EQ(4U, B.cols);

    Matrix *C = 4.0F * B;
    ASSERT_EQ(2U, C->rows);
    ASSERT_EQ(4U, C->cols);

    Matrix D({4,8,12,16,20,24,28,32});
    D.reshape(2U, 4U);
    ASSERT_EQ(2U, D.rows);
    ASSERT_EQ(4U, D.cols);
    ASSERT_EQ(D, *C);

    Matrix *E = B * 4.0;
    ASSERT_EQ(D, *E);

    Matrix *F = 4 * B;
    ASSERT_EQ(D, *F);

    Matrix *G = B * 4U;
    ASSERT_EQ(D, *G);

    Static::clean();
}

TEST(operators, multiply)
{
    Matrix A;
    ASSERT_EQ(0U, A.rows);
    ASSERT_EQ(0U, A.cols);
    ASSERT_EQ(0U, A.val.capacity());

    Matrix B({1,2,3,4,5,6,7,8,9,10,11,12});
    B.reshape(3U, 4U);
    ASSERT_EQ(3U, B.rows);
    ASSERT_EQ(4U, B.cols);
    ASSERT_EQ(12U, B.val.size());

    Matrix *C = A * B;
    ASSERT_EQ(nullptr, C);

    A.id(3U);
    ASSERT_EQ(3U, A.rows);
    ASSERT_EQ(3U, A.cols);

    C = 3 * A;
    ASSERT_EQ(3U, C->rows);
    ASSERT_EQ(3U, C->cols);
    Matrix D({3,0,0,0,3,0,0,0,3});
    D.reshape(3U, 3U);
    ASSERT_EQ(D, *C);
    std::cout << std::string(B) << std::endl;
    std::cout << std::string(*C) << std::endl;
    Matrix *E = *C * B;
    ASSERT_EQ(3U, E->rows);
    ASSERT_EQ(4U, E->cols);
    std::cout << std::string(*E) << std::endl;

    Matrix F({3,6,9,12,15,18,21,24,27,30,33,36});
    F.reshape(3U, 4U);
    std::cout << std::string(F) << std::endl;
    ASSERT_EQ(3U, F.rows);
    ASSERT_EQ(4U, F.cols);
    ASSERT_EQ(F, *E);
    Static::clean();
}
*/
