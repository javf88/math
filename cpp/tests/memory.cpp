/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <cstdint>
#include <gtest/gtest.h>
/* TARGET LIBRARY */
#include "memory.hpp"

/******************************************************************************/
/*    FIXTURES                                                                */
/******************************************************************************/

class DummyData: public testing::Test
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

TEST(stack, getStack)
{
    std::stack<void*> *pStack = nullptr;
    ASSERT_EQ(pStack, nullptr);

    pStack = Static::getStack();
    ASSERT_NE(pStack, nullptr);
}

TEST_F(DummyData, SetUp)
{
    std::stack<void*> *pStack = Static::getStack();
    ASSERT_NE(pStack, nullptr);
    ASSERT_EQ(pStack->empty(), false);

    Static::clean();
    ASSERT_EQ(pStack->empty(), true);
    ASSERT_EQ(pStack->size(), 0U);
}
