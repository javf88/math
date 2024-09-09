/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include "unity.h"
/* TARGET LIBRARY */
#include "algebra.h"
#include <unity_internals.h>

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

void test_transpose(void)
{
    MATRIX *A =push_matrix(2U, 4U);
    TEST_ASSERT_NOT_NULL(A);

    for (uint32_t i = 0; i < 2U; i++)
    {
        for (uint32_t j = 0; j < 4U; j++)
        {
            A->val[4U * i + j] = 4U * i + j;
        }
    }

    MATRIX *B = transpose(A);
    TEST_ASSERT_NOT_NULL(B);

    for (uint32_t i = 0; i < 2U; i++)
    {
        for (uint32_t j = 0; j < 4U; j++)
        {
            uint32_t pos = A->cols * i + j;
            uint32_t posT = A->rows * j + i;

            TEST_ASSERT_EQUAL_FLOAT(A->val[pos], B->val[posT]);
        }
    }

    do {
        /* The stack starts with a non-NULL value */
        TEST_ASSERT_NOT_NULL(stack);
        stack = pop_matrix(stack);
    } while(stack != NULL);
}

void test_add(void)
{
    MATRIX *A = push_matrix(3U, 5U);
    TEST_ASSERT_NOT_NULL(A);

    MATRIX *B = push_matrix(3U, 5U);
    TEST_ASSERT_NOT_NULL(B);

    for (uint32_t i = 0; i < 3U; i++)
    {
        for (uint32_t j = 0; j < 5U; j++)
        {
            A->val[5U * i + j] = 5U * i + j;
            B->val[5U * i + j] = 5U * i + j;
        }
    }

    MATRIX *C = add(A, B);
    TEST_ASSERT_NOT_NULL(C);

    for (uint32_t i = 0; i < 3U; i++)
    {
        for (uint32_t j = 0; j < 5U; j++)
        {
            TEST_ASSERT_EQUAL_FLOAT(C->val[5U * i + j], 2U * (5U * i + j));
        }
    }

    do {
        /* The stack starts with a non-NULL value */
        TEST_ASSERT_NOT_NULL(stack);
        stack = pop_matrix(stack);
    } while(stack != NULL);
}
void test_subs(void)
{
    MATRIX *A = push_matrix(5U, 3U);
    TEST_ASSERT_NOT_NULL(A);

    MATRIX *B = push_matrix(5U, 3U);
    TEST_ASSERT_NOT_NULL(B);

    for (uint32_t i = 0; i < 5U; i++)
    {
        for (uint32_t j = 0; j < 3U; j++)
        {
            A->val[3U * i + j] = 3U * i + j;
            B->val[3U * i + j] = 3U * i + j;
        }
    }

    MATRIX *C = subs(A, B);
    TEST_ASSERT_NOT_NULL(C);

    for (uint32_t i = 0; i < 5U; i++)
    {
        for (uint32_t j = 0; j < 3U; j++)
        {
            TEST_ASSERT_EQUAL_FLOAT(C->val[3U * i + j], 0U);
        }
    }

    do {
        /* The stack starts with a non-NULL value */
        TEST_ASSERT_NOT_NULL(stack);
        stack = pop_matrix(stack);
    } while(stack != NULL);
}

void test_mult(void)
{
    MATRIX *A = push_matrix(4U, 1U);
    TEST_ASSERT_NOT_NULL(A);

    for (uint32_t i = 0; i < 4U; i++)
    {
        A->val[i] = 1;
    }

    MATRIX *AT = transpose(A);
    TEST_ASSERT_NOT_NULL(AT);

    MATRIX *ATA = mult(AT,A);
    TEST_ASSERT_NOT_NULL(ATA);
    TEST_ASSERT_EQUAL_UINT32(1U, ATA->rows);
    TEST_ASSERT_EQUAL_UINT32(1U, ATA->cols);
    TEST_ASSERT_EQUAL_FLOAT(4.0F, ATA->val[0]);

    A = push_matrix(3U, 7U);
    MATRIX *B = push_matrix(7U, 4U);

    for (uint32_t i = 0U; i < 3U * 7U; i++)
    {
        A->val[i] = 1.0F;
    }

    for (uint32_t i = 0U; i < 7U * 4U; i++)
    {
        B->val[i] = 2.0F;
    }

    MATRIX *C = mult(A, B);
    TEST_ASSERT_EQUAL_UINT32(3U, C->rows);
    TEST_ASSERT_EQUAL_UINT32(4U, C->cols);

    /* Wrong matrix multiplication, wait for log system to debug better */
    _print(A);
    _print(B);
    _print(C);

    for (uint32_t i = 0U; i < 3U; i++)
    {
        for (uint32_t j = 0U; j < 4U; j++)
        {
            TEST_ASSERT_EQUAL_FLOAT(14.0F, C->val[4U * i + j]);
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

    RUN_TEST(test_transpose);
    RUN_TEST(test_add);
    RUN_TEST(test_mult);

    return UNITY_END();
}
