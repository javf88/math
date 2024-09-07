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
    MATRIX *A = _malloc(4U, 7U);
    TEST_ASSERT_NOT_NULL(A);
    TEST_ASSERT_EQUAL_UINT32(4U, A->rows);
    TEST_ASSERT_EQUAL_UINT32(7U, A->cols);
    TEST_ASSERT_NOT_NULL(A->val);

    A = _free(A);
    TEST_ASSERT_NULL(A);
}

void test_create_matrices(void)
{
    for (uint32_t i = 0; i < 5U; i++)
    {
        for (uint32_t j = 0; j < 5U; j++)
        {
            MATRIX *A = create_matrix(i, j);
            TEST_ASSERT_NOT_NULL(A);
            TEST_ASSERT_EQUAL_UINT32(i, A->rows);
            TEST_ASSERT_EQUAL_UINT32(j, A->cols);
            TEST_ASSERT_NOT_NULL(A->val);
        }
    }
}

void test_free_matrices(void)
{
    list = free_matrices(list);
    TEST_ASSERT_NULL(list);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_valid_malloc_and_free);
    RUN_TEST(test_create_matrices);
    RUN_TEST(test_free_matrices);

    return UNITY_END();
}
