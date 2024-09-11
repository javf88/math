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

    sprintf(buffer, "\x1b[31m[ ERROR ]\x1b[0m %s:%d", __FILE__, 44);
    actual = _get_src(0, __FILE__, 44);
    TEST_ASSERT_EQUAL_STRING(actual, buffer);
    free(actual);

    sprintf(buffer, "\x1b[33m[WARNING]\x1b[0m %s:%d", __FILE__, 48);
    actual = _get_src(1, __FILE__, 48);
    TEST_ASSERT_EQUAL_STRING(actual, buffer);
    free(actual);

    sprintf(buffer, "\x1b[32m[ INFO  ]\x1b[0m %s:%d", __FILE__, 52);
    actual = _get_src(2, __FILE__, 52);
    TEST_ASSERT_EQUAL_STRING(actual, buffer);
    free(actual);

    sprintf(buffer, "\x1b[36m[ DEBUG ]\x1b[0m %s:%d", __FILE__, 56);
    actual = _get_src(3, __FILE__, 56);
    TEST_ASSERT_EQUAL_STRING(actual, buffer);
    free(actual);

    sprintf(buffer, "\x1b[94m[ TRACE ]\x1b[0m %s:%d", __FILE__, 60);
    actual = _get_src(4, __FILE__, 60);
    TEST_ASSERT_EQUAL_STRING(actual, buffer);
    free(actual);

    free(buffer);
}

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

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_get_src);
    RUN_TEST(test_get_msg);

    return UNITY_END();
}
