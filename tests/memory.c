/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include "unity.h"
/* TARGET LIBRARY */
#include "memory.h"

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

void test_valid_malloc_and_free(void)
{
    MATRIX *A = _M2S_malloc(4U, 7U);
    TEST_ASSERT_NOT_NULL(A);
    TEST_ASSERT_EQUAL_UINT32(4U, A->rows);
    TEST_ASSERT_EQUAL_UINT32(7U, A->cols);
    TEST_ASSERT_NOT_NULL(A->val);

    A = _M2S_free(A);
    TEST_ASSERT_NULL(A);
}

void test_matrix_push_and_pop(void)
{
    TEST_ASSERT_NULL(stack);

    MATRIX *A = push_matrix(3U, 4U);
    TEST_ASSERT_NOT_NULL(A);
    TEST_ASSERT_NOT_NULL(stack);

    stack = pop_matrix(stack);
    TEST_ASSERT_NULL(stack);
}

void test_push_and_pop(void)
{
    for (uint32_t i = 0; i < 5U; i++)
    {
        for (uint32_t j = 0; j < 5U; j++)
        {
            MATRIX *A = push_matrix(i, j);
            TEST_ASSERT_NOT_NULL(A);
            TEST_ASSERT_EQUAL_UINT32(i, A->rows);
            TEST_ASSERT_EQUAL_UINT32(j, A->cols);
            TEST_ASSERT_NOT_NULL(A->val);
        }
    }

    do {
        /* The stack starts with a non-NULL value */
        TEST_ASSERT_NOT_NULL(stack);
        stack = pop_matrix(stack);
    } while(stack != NULL);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_valid_malloc_and_free);
    RUN_TEST(test_matrix_push_and_pop);
    RUN_TEST(test_push_and_pop);

    return UNITY_END();
}
