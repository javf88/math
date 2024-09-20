/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <unistd.h>

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

void test_init(void)
{
    /* Wrong (LOG*)1U value */
    LOG *nullPtr = init((LOG*)1U, 0U);
    TEST_ASSERT_EQUAL_PTR ((LOG*)1U, nullPtr);

    /* only stderr as target for logging */
    /* LOG_TO_FILE is either 0U or 1U */
    LOG *files = init(file, 0U);
    TEST_ASSERT_NOT_NULL(files);

    uint32_t count = 0U;
    for (LOG *itr = files; itr->name != NULL; itr = itr + sizeof(LOG))
    {
        TEST_ASSERT_NOT_NULL(itr);
        TEST_ASSERT_EQUAL_STRING("stderr", itr->name);
        TEST_ASSERT_EQUAL_PTR(stderr, itr->descriptor);
        TEST_ASSERT_EQUAL_UINT32(0U, count);
        count++;
    }

    /* a file and the stderr as targets for logging */
    LOG *tmp = files = init(files, 1U);
    TEST_ASSERT_NOT_NULL(files);

    count = 0U;
    for (LOG *itr = files; itr->name != NULL; itr = itr + sizeof(LOG))
    {
        TEST_ASSERT_NOT_NULL(itr);
        TEST_ASSERT_NOT_NULL(itr->name);
        TEST_ASSERT_NOT_NULL(itr->descriptor);
        count++;
    }
    TEST_ASSERT_EQUAL_UINT32(1U, count);
}

void test_make(void)
{
    TEST_ASSERT_EQUAL_INT32(0, make("dummy"));
    TEST_ASSERT_EQUAL_INT32(17, make("dummy"));
    TEST_ASSERT_EQUAL_INT32(2, make("test/dummy"));

    /* maybe at tearDown */
    remove("dummy");
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_init);
    RUN_TEST(test_make);

    return UNITY_END();
}
