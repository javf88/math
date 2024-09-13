/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include "unity.h"
/* TARGET LIBRARY */
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

void test_get_src(void)
{
    char *buffer = malloc(sizeof(char) * 256U);
    char *actual = NULL;

    sprintf(buffer, RED("[ ERROR ] %s:%d"), __FILE__, 44);
    actual = _get_src(0, __FILE__, 44);
    TEST_ASSERT_EQUAL_STRING(actual, buffer);
    free(actual);

    sprintf(buffer, YELLOW("[WARNING] %s:%d"), __FILE__, 48);
    actual = _get_src(1, __FILE__, 48);
    TEST_ASSERT_EQUAL_STRING(actual, buffer);
    free(actual);

    sprintf(buffer, GREEN("[ INFO  ] %s:%d"), __FILE__, 52);
    actual = _get_src(2, __FILE__, 52);
    TEST_ASSERT_EQUAL_STRING(actual, buffer);
    free(actual);

    sprintf(buffer, CYAN("[ DEBUG ] %s:%d"), __FILE__, 56);
    actual = _get_src(3, __FILE__, 56);
    TEST_ASSERT_EQUAL_STRING(actual, buffer);
    free(actual);

    sprintf(buffer, PURPLE("[ TRACE ] %s:%d"), __FILE__, 60);
    actual = _get_src(4, __FILE__, 60);
    TEST_ASSERT_EQUAL_STRING(actual, buffer);
    free(actual);

    free(buffer);
}

/* helper function to simulate variadic function */
char* create_variadic_args(const char *format, ...)
{
    char * actual = NULL;
    va_list args;

    va_start(args, format);
    actual = _get_msg(format, args);
    va_end(args);

    return actual;
}

void test_get_msg(void)
{
    char *actual = create_variadic_args("%s:%d %s", "test", 87, "END");

    char *buffer = malloc(sizeof(char) * 256U);
    sprintf(buffer, "%s:%d %s", "test", 87, "END");

    TEST_ASSERT_EQUAL_STRING(actual, buffer);

    free(actual);
    free(buffer);
}

void test_examples(void)
{
    LOG_ERROR("This should be an error! LOG_LEVEL_ERROR: %u", LOG_LEVEL_ERROR);
    LOG_WARNING("This should be an warning! LOG_LEVEL_WARNING: %u", LOG_LEVEL_WARNING);
    LOG_INFO("This should be an info! LOG_LEVEL_INFO: %u", LOG_LEVEL_INFO);
    LOG_DEBUG("This should be an debug! LOG_LEVEL_DEBUG: %u", LOG_LEVEL_DEBUG);
    LOG_TRACE("This should be an trace! LOG_LEVEL_TRACE: %u", LOG_LEVEL_TRACE);

}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_get_src);
    RUN_TEST(test_get_msg);
    RUN_TEST(test_examples);

    return UNITY_END();
}
