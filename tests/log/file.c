/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <stdio.h>
#include <time.h>
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

    /* LOG_TO_FILE is either 0U or 1U */
    /* len(file) == 2U */
    file = init(file, 0U);
    TEST_ASSERT_NOT_NULL(file);
    TEST_ASSERT_EQUAL_STRING("stderr", file[0U].name);
    TEST_ASSERT_EQUAL_PTR(stderr, file[0U].descriptor);
    TEST_ASSERT_NULL(file[1U].name);
    TEST_ASSERT_NULL(file[1U].descriptor);
    free(file);
    file = NULL;

    /* len(file) == 3U */
    file = init(file, 1U);
    TEST_ASSERT_NOT_NULL(file);
    TEST_ASSERT_NULL(file[0U].name);
    TEST_ASSERT_NULL(file[0U].descriptor);
    TEST_ASSERT_EQUAL_STRING("stderr", file[1U].name);
    TEST_ASSERT_EQUAL_PTR(stderr, file[1U].descriptor);
    TEST_ASSERT_NULL(file[2U].name);
    TEST_ASSERT_NULL(file[2U].descriptor);
    free(file);
    file = NULL;

}

void test_make(void)
{
    TEST_ASSERT_EQUAL_INT32(0, make("dummy"));
    TEST_ASSERT_EQUAL_INT32(17, make("dummy"));
    TEST_ASSERT_EQUAL_INT32(2, make("test/dummy"));

    /* maybe at tearDown */
    remove("dummy");
}

void test_name(void)
{
    LOG newFile;
    time_t secs;
    char filename[50U];

    time(&secs);
    name(&newFile);
    sprintf(filename, "tmp/%ld.log", secs);
    TEST_ASSERT_EQUAL_STRING(filename, newFile.name);

    remove("tmp");
}

void test_open(void)
{
    char filename[] = "test.log";
    LOG newFile = {NULL, filename};
    TEST_ASSERT_EQUAL_PTR(&newFile, open(&newFile));
}

void test_constructor(void)
{
    /* LOG_TO_FILE is 1U due to the buildsystem */
    time_t secs;
    char filename[50U];

    /* Trying to get the same name, race condition */
    time(&secs);
    sprintf(filename, "tmp/%ld.log", secs);

    constructor();
    TEST_ASSERT_EQUAL_STRING(filename, file[0U].name);
    TEST_ASSERT_NOT_NULL(file[0U].descriptor);
    TEST_ASSERT_EQUAL_STRING("stderr", file[1U].name);
    TEST_ASSERT_EQUAL_PTR(stderr, file[1U].descriptor);

    free(file[0U].name);
    free(file);
    file = NULL;
}

void test_destructor(void)
{
    constructor();

    destructor();
    TEST_ASSERT_NULL(file);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_init);
    RUN_TEST(test_make);
    RUN_TEST(test_name);
    RUN_TEST(test_open);
    RUN_TEST(test_constructor);
    RUN_TEST(test_destructor);

    return UNITY_END();
}
