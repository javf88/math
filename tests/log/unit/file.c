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

void test_get(void)
{
    const LOG *files = get();;
    TEST_ASSERT_NOT_NULL(files);
    /* Default stream */
    TEST_ASSERT_NOT_NULL(files[0U].descriptor);
    TEST_ASSERT_NOT_NULL(files[0U].name);
    TEST_ASSERT_EQUAL_STRING("stderr", files[0U].name);
    /* Log file placeholder with inital vals as NULL */
    TEST_ASSERT_NULL(files[1U].descriptor);
    TEST_ASSERT_NULL(files[1U].name);
    /* Sentinel element */
    TEST_ASSERT_NULL(files[2U].descriptor);
    TEST_ASSERT_NULL(files[2U].name);
}

void test_set(void)
{
    LOG newFile = {stdout, "stdout"};
    LOG *files = set(&newFile);
    TEST_ASSERT_NOT_NULL(files);
    /* Default stream */
    TEST_ASSERT_NOT_NULL(files[0U].descriptor);
    TEST_ASSERT_NOT_NULL(files[0U].name);
    TEST_ASSERT_EQUAL_STRING("stderr", files[0U].name);
    /* new stream */
    TEST_ASSERT_NOT_NULL(files[1U].descriptor);
    TEST_ASSERT_NOT_NULL(files[1U].name);
    TEST_ASSERT_EQUAL_STRING("stdout", files[1U].name);
    /* Sentinel */
    TEST_ASSERT_NULL(files[2U].descriptor);
    TEST_ASSERT_NULL(files[2U].name);

    files = set((LOG*)NULL);
    /* reset to inital vals (NULL) */
    TEST_ASSERT_NULL(files[1U].descriptor);
    TEST_ASSERT_NULL(files[1U].name);
}

void test_make(void)
{
    TEST_ASSERT_EQUAL_INT32(0, make("dummy"));
    TEST_ASSERT_EQUAL_INT32(17, make("dummy"));
    TEST_ASSERT_EQUAL_INT32(2, make("test/dummy"));

    /* maybe at tearDown */
    remove("dummy");
}

void test_make_name(void)
{
    char buffer[256U];
    time_t secs;
    time(&secs);
    const char *format = "tmp/%ld.log";
    char *name = make_name(format);

    sprintf(buffer, format, secs);
    TEST_ASSERT_EQUAL_STRING(buffer, name);
}

void test_open(void)
{
    char *filename = make_name("test.log");

    LOG *file = open(filename);
    TEST_ASSERT_NOT_NULL(file);
    TEST_ASSERT_NOT_NULL(file->descriptor);
    TEST_ASSERT_EQUAL_STRING("test.log", file->name);

    remove(filename);
}

void test_init(void)
{
    /* LOG_TO_FILE OFF */
    LOG *files = init(0U);
    TEST_ASSERT_NOT_NULL(files);
    /* Default stream */
    TEST_ASSERT_NOT_NULL(files[0U].descriptor);
    TEST_ASSERT_NOT_NULL(files[0U].name);
    TEST_ASSERT_EQUAL_STRING("stderr", files[0U].name);
    /* Log file placeholder with inital vals as NULL */
    TEST_ASSERT_NULL(files[1U].descriptor);
    TEST_ASSERT_NULL(files[1U].name);
    /* Sentinel element */
    TEST_ASSERT_NULL(files[2U].descriptor);
    TEST_ASSERT_NULL(files[2U].name);

    char buffer[256U];
    time_t secs;
    time(&secs);
    sprintf(buffer, "tmp/%ld.log", secs);
    /* LOG_TO_FILE ON */
    files = init(1U);
    TEST_ASSERT_NOT_NULL(files);
    /* Default stream */
    TEST_ASSERT_NOT_NULL(files[0U].descriptor);
    TEST_ASSERT_NOT_NULL(files[0U].name);
    TEST_ASSERT_EQUAL_STRING("stderr", files[0U].name);
    /* new stream */
    TEST_ASSERT_NOT_NULL(files[1U].descriptor);
    TEST_ASSERT_NOT_NULL(files[1U].name);
    TEST_ASSERT_EQUAL_STRING(buffer, files[1U].name);
    /* Sentinel */
    TEST_ASSERT_NULL(files[2U].descriptor);
    TEST_ASSERT_NULL(files[2U].name);

    /* Clean up */
    remove(files[1U].name);
    remove("tmp");
}

void test_constructor(void)
{
    LOG *files = get();

    time_t secs;
    char filename[50U];

    /* Trying to get the same name, race condition */
    time(&secs);
    sprintf(filename, "tmp/%ld.log", secs);

    constructor();
    TEST_ASSERT_NOT_NULL(files[1U].descriptor);
    TEST_ASSERT_EQUAL_STRING(filename, files[1U].name);

    remove(filename);
    remove("tmp");
}

void test_destructor(void)
{
    char name[256U];
    LOG *file = get();
    constructor();

    sprintf(name, "%s", file[1U].name);

    destructor();
    remove(name);
    remove("tmp");
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_get);
    RUN_TEST(test_set);
    RUN_TEST(test_make);
    RUN_TEST(test_make_name);
    RUN_TEST(test_open);
    RUN_TEST(test_init);
    RUN_TEST(test_constructor);
    RUN_TEST(test_destructor);

    return UNITY_END();
}
