/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include "unity.h"
#include "levels.h"
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
    MATRIX *A = matrix_malloc(4U, 7U);
    TEST_ASSERT_NOT_NULL(A);
    TEST_ASSERT_EQUAL_UINT32(4U, A->rows);
    TEST_ASSERT_EQUAL_UINT32(7U, A->cols);
    TEST_ASSERT_NOT_NULL(A->val);

    A = matrix_free(A);
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

void test_get_block_matrix(void)
{
    MATRIX *A = push_matrix(6U, 6U);
    TEST_ASSERT_NOT_NULL(A);
    float val[36U] = { 1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F,
                       1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F,
                       1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F,
                       1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F,
                       1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F,
                       1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F};
    memcpy(A->val, val, sizeof(float) * A->cols * A->rows);
    LOG_INFO_MATRIX(A);

    /* NULL matrix */
    TEST_ASSERT_NULL(get_block_matrix(NULL, 1U, 2U, 3U, 4U));
    /* rowEnd > A->rows */
    TEST_ASSERT_EQUAL_PTR(A, get_block_matrix(A, 1U, 20U, 3U, 4U));
    /* colEnd > A->cols */
    TEST_ASSERT_EQUAL_PTR(A, get_block_matrix(A, 1U, 2U, 3U, 40U));
    /* row > rowEnd */
    TEST_ASSERT_EQUAL_PTR(A, get_block_matrix(A, 3U, 2U, 3U, 4U));
    /* col > colEnd */
    TEST_ASSERT_EQUAL_PTR(A, get_block_matrix(A, 1U, 2U, 5U, 4U));

    /* Getting a scalar  */
    MATRIX *subA = get_block_matrix(A, 1U, 2U, 3U, 4U);
    TEST_ASSERT_EQUAL_UINT32(subA->rows, 1U);
    TEST_ASSERT_EQUAL_UINT32(subA->cols, 1U);
    TEST_ASSERT_EQUAL_FLOAT(4.0F, subA->val[0U]);
    LOG_INFO_MATRIX(subA);

    /* Getting block matrix */
    subA = get_block_matrix(A, 1U, 4U, 3U, A->cols);
    float expVal[9U] = {4.0F, 5.0F, 6.0F, 4.0F, 5.0F, 6.0F, 4.0F, 5.0F, 6.0F};
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(subA->val, expVal, subA->cols * subA->rows);
    LOG_INFO_MATRIX(subA);

    /* Getting row vector */
    subA = GET_ROW_VECTOR(A, 3U);
    float expVec[6U] = {1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F};
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(subA->val, expVec, subA->cols * subA->rows);
    LOG_INFO_MATRIX(subA);

    /* Getting column vector */
    subA = GET_COLUMN_VECTOR(A, 4U);
    float expCol[6U] = {5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F};
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(subA->val, expCol, subA->cols * subA->rows);
    LOG_INFO_MATRIX(subA);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_valid_malloc_and_free);
    RUN_TEST(test_matrix_push_and_pop);
    RUN_TEST(test_push_and_pop);
    RUN_TEST(test_get_block_matrix);

    return UNITY_END();
}
