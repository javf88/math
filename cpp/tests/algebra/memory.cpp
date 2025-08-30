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
    Memory<void*> list;

    void SetUp() override
    {
        using namespace std;

        // To improve logging message.
        LOG_INFO(list.logMemory, "Running SetUp()");

        for (uint32_t i = 0U; i < 32U; i++)
        {
            void *pInt = new int(i);
            LOG_DEBUG(list.logMemory, "push_back(", pInt, ")");
            list.push_back(pInt);
        }
    }
};

/******************************************************************************/
/*    TEST CASES                                                              */
/******************************************************************************/

TEST_F(DummyList, clean)
{
    ASSERT_EQ(list.size(), 32U);
    ASSERT_EQ(list.empty(), false);

    list.clean();
    ASSERT_EQ(list.empty(), true);
    ASSERT_EQ(list.size(), 0U);
}
