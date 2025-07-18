/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <gtest/gtest.h>
/* TARGET LIBRARY */
#include "operators.hpp"
#include "memory.hpp"

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
    B.id(3U);
    ASSERT_EQ(false, A == B);
    A.id(3);
    ASSERT_EQ(A, B);
}

TEST(operators, add)
{
    Matrix *A = new Matrix;
    ASSERT_EQ(0U, A->rows);
    ASSERT_EQ(0U, A->cols);
    ASSERT_EQ(0U, A->val.capacity());

    Matrix *B = new Matrix({1,2,3,4,5,6,7,8});
    ASSERT_EQ(1U, B->rows);
    ASSERT_EQ(8U, B->cols);
    ASSERT_EQ(8U, B->val.size());

    Matrix *C = *A + *B;
    ASSERT_EQ(nullptr, C);

    B->reshape(2U, 4U);
    C = *B + *B;
    ASSERT_EQ(2U, C->rows);
    ASSERT_EQ(4U, C->cols);

    Matrix D({2,4,6,8,10,12,14,16});
    D.reshape(2U, 4U);
    ASSERT_EQ(D, *C);

    Static::clean();
}
