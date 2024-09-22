/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include "unity.h"
/* TARGET LIBRARY */
#include "operators.h"

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
    MATRIX *A = NULL;
    /* Wrong input */
    A = transpose(A);
    TEST_ASSERT_NULL(A);

    A = push_matrix(2U, 4U);
    TEST_ASSERT_NOT_NULL(A);

    for (uint32_t i = 0; i < A->rows; i++)
    {
        for (uint32_t j = 0; j < A->cols; j++)
        {
            uint32_t pos = A(i, j);
            A->val[pos] = pos;
        }
    }

    MATRIX *B = transpose(A);
    TEST_ASSERT_NOT_NULL(B);

    for (uint32_t i = 0; i < A->rows; i++)
    {
        for (uint32_t j = 0; j < A->cols; j++)
        {
            uint32_t pos = A(i, j);
            uint32_t posT = AT(i, j);

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

    for (uint32_t i = 0; i < A->rows; i++)
    {
        for (uint32_t j = 0; j < A->cols; j++)
        {
            A->val[A->cols * i + j] = A->cols * i + j;
            B->val[A->cols * i + j] = A->cols * i + j;
        }
    }

    MATRIX *C = add(A, B);
    TEST_ASSERT_NOT_NULL(C);
    C = add(A, NULL);
    TEST_ASSERT_NULL(C);

    for (uint32_t i = 0; i < 3U; i++)
    {
        for (uint32_t j = 0; j < 5U; j++)
        {
            TEST_ASSERT_EQUAL_FLOAT(C->val[5U * i + j], 2U * (5U * i + j));
        }
    }

    MATRIX *D = push_matrix(2U, 2U);
    MATRIX *E = add(C, D);
    TEST_ASSERT_NULL(E);

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

    MATRIX *D = push_matrix(2U, 2U);
    MATRIX *E = subs(B, D);
    TEST_ASSERT_NULL(E);

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

    for (uint32_t i = 0U; i < 3U; i++)
    {
        for (uint32_t j = 0U; j < 4U; j++)
        {
            TEST_ASSERT_EQUAL_FLOAT(14.0F, C->val[4U * i + j]);
        }
    }

    MATRIX *D = push_matrix(2U, 2U);
    MATRIX *E = mult(D, E);
    TEST_ASSERT_NULL(E);

    do {
        /* The stack starts with a non-NULL value */
        TEST_ASSERT_NOT_NULL(stack);
        stack = pop_matrix(stack);
    } while(stack != NULL);
}

void test_id(void)
{
    MATRIX *A = id(0U);
    TEST_ASSERT_NOT_NULL(A);

    MATRIX *B = id(4U);
    TEST_ASSERT_NOT_NULL(B);

    for (uint32_t i = 0U; i < B->rows; i++)
    {
        for (uint32_t j = 0U; j < B->cols; j++)
        {
            if (i == j)
            {
                TEST_ASSERT_EQUAL_FLOAT(1.0F, B->val[B->cols * i + j]);
            }
            else
            {
                TEST_ASSERT_EQUAL_FLOAT(0.0F, B->val[B->cols * i + j]);
            }
        }
    }

    do {
        /* The stack starts with a non-NULL value */
        TEST_ASSERT_NOT_NULL(stack);
        stack = pop_matrix(stack);
    } while(stack != NULL);
}

void test_permute(void)
{
    MATRIX *I = id(5U);
    float expI[25U] = {0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 1.0000000F, 0.0000000F, 1.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 1.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 1.0000000F, 0.0000000F, 1.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F};

    permute(I, 2U, 5U);
    TEST_ASSERT_NOT_NULL(I);
    LOG_INFO_MATRIX(I);

    permute(I, 4U, 0U);
    for (uint32_t i = 0U; i < 5U * 5U; i++)
    {
        TEST_ASSERT_EQUAL_FLOAT(expI[i], I->val[i]);
    }
    LOG_INFO_MATRIX(I);

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
//    RUN_TEST(test_add);
//    RUN_TEST(test_subs);
//    RUN_TEST(test_mult);
//    RUN_TEST(test_id);
//    RUN_TEST(test_permute);

    return UNITY_END();
}
