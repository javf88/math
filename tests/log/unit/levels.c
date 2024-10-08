/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include "unity.h"
/* TARGET LIBRARIES */
#include "memory.h"
#include "levels.h"

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
    char *buffer = malloc(sizeof(char) * MAX_STR_LEN);
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

    char *buffer = malloc(sizeof(char) * MAX_STR_LEN);
    sprintf(buffer, "%s:%d %s", "test", 87, "END");

    TEST_ASSERT_EQUAL_STRING(actual, buffer);

    free(actual);
    free(buffer);
}

void test_tee_printf(void)
{
    char filename[256U];
    time_t secs;

    time(&secs);
    sprintf(filename, "tmp/%ld.log", secs);
    constructor();
    destructor();

    /* Indirect test, by removing we make sure next test-run does not break */
    remove(filename);
    remove("tmp");
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_get_src);
    RUN_TEST(test_get_msg);
    RUN_TEST(test_tee_printf);

    return UNITY_END();
}
