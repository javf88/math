/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <gtest/gtest.h>
/* TARGET LIBRARY */
#include "memory.hpp"

/******************************************************************************/
/*    FIXTURES                                                                */
/******************************************************************************/

class DummyList: public testing::Test
{
public:
    void SetUp() override
    {
        using namespace std;

        // To improve logging message.
        LOG_INFO(Static::logMemory, "Running SetUp()");

        deque<void*> *pList = Static::getList();
        for (uint32_t i = 0U; i < 32U; i++)
        {
            void *pInt = new int(i);
            LOG_DEBUG(Static::logMemory, "push_back(", pInt, ")");
            pList->push_back(pInt);
        }
    }
};

/******************************************************************************/
/*    TEST CASES                                                              */
/******************************************************************************/

TEST(StaticList, getList)
{
    std::deque<void*> *pList = nullptr;
    ASSERT_EQ(pList, nullptr);

    pList = Static::getList();
    ASSERT_NE(pList, nullptr);
}

TEST_F(DummyList, clean)
{
    std::deque<void*> *pList = Static::getList();
    ASSERT_NE(pList, nullptr);
    ASSERT_EQ(pList->empty(), false);

    Static::clean();
    ASSERT_EQ(pList->empty(), true);
    ASSERT_EQ(pList->size(), 0U);
}
