/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include "unity.h"
/* TARGET LIBRARIES */
#include "memory.h"
/* A very bad practice, but this is for testing the static functions, rather
 * than doubling declarations and prototypes */
#include "levels.c"

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
    actual = get_src(0, __FILE__, 44);
    TEST_ASSERT_EQUAL_STRING(actual, buffer);
    free(actual);

    sprintf(buffer, YELLOW("[WARNING] %s:%d"), __FILE__, 48);
    actual = get_src(1, __FILE__, 48);
    TEST_ASSERT_EQUAL_STRING(actual, buffer);
    free(actual);

    sprintf(buffer, GREEN("[ INFO  ] %s:%d"), __FILE__, 52);
    actual = get_src(2, __FILE__, 52);
    TEST_ASSERT_EQUAL_STRING(actual, buffer);
    free(actual);

    sprintf(buffer, CYAN("[ DEBUG ] %s:%d"), __FILE__, 56);
    actual = get_src(3, __FILE__, 56);
    TEST_ASSERT_EQUAL_STRING(actual, buffer);
    free(actual);

    sprintf(buffer, PURPLE("[ TRACE ] %s:%d"), __FILE__, 60);
    actual = get_src(4, __FILE__, 60);
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
    actual = get_msg(format, args);
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

    RUN_TEST(test_get_src);
    RUN_TEST(test_get_msg);
    RUN_TEST(test_log_matrix);

    return UNITY_END();
}
