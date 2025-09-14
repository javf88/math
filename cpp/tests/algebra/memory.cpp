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
    Memory<void*> manager;

    void SetUp() override
    {
        using namespace std;

        // To improve logging message.
        LOG_INFO(manager.logMemory, "Running SetUp()");

        for (uint32_t i = 0U; i < 32U; i++)
        {
            void *pInt = new int(i);
            LOG_DEBUG(manager.logMemory, "push_back(", pInt, ")");
            manager.push_back(pInt);
        }
    }
};

/******************************************************************************/
/*    TEST CASES                                                              */
/******************************************************************************/

TEST_F(DummyList, clean)
{
    ASSERT_EQ(manager.size(), 32U);
    ASSERT_EQ(manager.empty(), false);

    manager.clean();
    ASSERT_EQ(manager.empty(), true);
    ASSERT_EQ(manager.size(), 0U);
}
