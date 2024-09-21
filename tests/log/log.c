/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include "unity.h"
/* TARGET LIBRARY */
#include "file.h"

/******************************************************************************/
/*    PRELUDE                                                                 */
/******************************************************************************/

/* setUp() and tearDown() are required by Unity */
void setUp(void)
{
    return;
}

void tearDown(void)
{
    return;
}

/******************************************************************************/
/*    TEST FUNCTIONS                                                          */
/******************************************************************************/

void test_examples(void)
{
    LOG_TRACE("A file with several lines is generated!");
    LOG_ERROR("This should be an error! LOG_LEVEL_ERROR: %u", LOG_LEVEL_ERROR);
    LOG_WARNING("This should be an warning! LOG_LEVEL_WARNING: %u", LOG_LEVEL_WARNING);
    LOG_INFO("This should be an info! LOG_LEVEL_INFO: %u", LOG_LEVEL_INFO);
    LOG_DEBUG("This should be an debug! LOG_LEVEL_DEBUG: %u", LOG_LEVEL_DEBUG);
    LOG_TRACE("This should be an trace! LOG_LEVEL_TRACE: %u", LOG_LEVEL_TRACE);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_examples);

    return UNITY_END();
}
