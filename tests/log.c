/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <unistd.h>

#include "unity.h"
#include "memory.h"
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

void test_file_init(void)
{
    LOG *files = init(file);
    TEST_ASSERT_NOT_NULL(files);
    TEST_ASSERT_NULL(files[0U].descriptor);
    TEST_ASSERT_NULL(files[0U].name);
    TEST_ASSERT_EQUAL_PTR(stderr, files[1U].descriptor);
    TEST_ASSERT_EQUAL_STRING("stderr", files[1U].name);
    TEST_ASSERT_NULL(files[2U].descriptor);
    TEST_ASSERT_NULL(files[2U].name);
    /*
    file = _file_init(file);
    */
}

void test_examples(void)
{
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

void test_file_close(void)
{
    /*
    file = _file_close(file);
    */
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_get_src);
    RUN_TEST(test_get_msg);
    RUN_TEST(test_file_init);
    /*
    RUN_TEST(test_examples);
    RUN_TEST(test_log_matrix);
    RUN_TEST(test_file_init);
    RUN_TEST(test_file_close);
    */
    return UNITY_END();
}
