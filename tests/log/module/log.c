/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include "unity.h"
/* TARGET LIBRARIES */
#include "memory.h"
#include "log.h"

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
    LOG_INFO("A file with several lines is generated!");
    LOG_ERROR("This should be an error! LOG_LEVEL_ERROR: %u", LOG_LEVEL_ERROR);
    LOG_WARNING("This should be an warning! LOG_LEVEL_WARNING: %u", LOG_LEVEL_WARNING);
    LOG_INFO("This should be an info! LOG_LEVEL_INFO: %u", LOG_LEVEL_INFO);
    LOG_DEBUG("This should be an debug! LOG_LEVEL_DEBUG: %u", LOG_LEVEL_DEBUG);
    LOG_TRACE("This should be an trace! LOG_LEVEL_TRACE: %u", LOG_LEVEL_TRACE);
}

void test_log_matrix(void)
{
    MATRIX *A = push_matrix(5U, 5U);
    /* TODO: need to add write to file, and compare against */
    LOG_INFO_MATRIX(A);

    do {
        /* The stack starts with a non-NULL value */
        TEST_ASSERT_NOT_NULL(stack);
        stack = pop_matrix(stack);
    } while(stack != NULL);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_examples);
    RUN_TEST(test_log_matrix);

    return UNITY_END();
}
