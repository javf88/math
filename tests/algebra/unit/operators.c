/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include "levels.h"
#include "unity.h"
#include "utilities.h"
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
    do {
        /* The stack starts with a non-NULL value */
        TEST_ASSERT_NOT_NULL(stack);
        stack = pop_matrix(stack);
    } while(stack != NULL);

    return;
}

__attribute__((constructor)) void init_submodule(void)
{
    log_init(__FILE__);
}

/******************************************************************************/
/*    TEST FUNCTIONS                                                          */
/******************************************************************************/

void test_transpose(void)
{
    MATRIX *A = NULL;
    log_info(__FUNCTION__);

    /* Wrong input */
    A = transpose(A);
    TEST_ASSERT_NULL(A);

    A = push_matrix(2U, 4U);
    TEST_ASSERT_NOT_NULL(A);

    for (uint32_t i = 0; i < A->rows; i++)
    {
        for (uint32_t j = 0; j < A->cols; j++)
        {
            uint32_t pos = TO_C_CONT(A, i, j);
            A->val[pos] = pos;
        }
    }
    LOG_DEBUG_MATRIX(A);
    MATRIX *B = transpose(A);
    TEST_ASSERT_NOT_NULL(B);
    LOG_DEBUG("B = AT");
    LOG_DEBUG_MATRIX(B);

    for (uint32_t i = 0; i < A->rows; i++)
    {
        for (uint32_t j = 0; j < A->cols; j++)
        {
            uint32_t pos = TO_C_CONT(A, i, j);
            uint32_t posT = TO_F_CONT(A, i, j);

            TEST_ASSERT_EQUAL_FLOAT(A->val[pos], B->val[posT]);
        }
    }
}

void test_add(void)
{
    MATRIX *A = push_matrix(3U, 5U);
    log_info(__FUNCTION__);

    TEST_ASSERT_NOT_NULL(A);

    MATRIX *B = push_matrix(3U, 5U);
    TEST_ASSERT_NOT_NULL(B);

    for (uint32_t i = 0; i < A->rows; i++)
    {
        for (uint32_t j = 0; j < A->cols; j++)
        {
            uint32_t pos = TO_C_CONT(A, i, j);
            A->val[pos] = B->val[pos] = pos;
        }
    }

    /* Wrong input */
    MATRIX *C = add(A, NULL);
    TEST_ASSERT_NULL(C);

    C = add(NULL, B);
    TEST_ASSERT_NULL(C);

    C = add(NULL, NULL);
    TEST_ASSERT_NULL(C);

    LOG_DEBUG_MATRIX(A);
    LOG_DEBUG_MATRIX(B);
    C = add(A, B);
    LOG_DEBUG("C = A + B");
    LOG_DEBUG_MATRIX(C);
    for (uint32_t i = 0; i < A->rows; i++)
    {
        for (uint32_t j = 0; j < A->cols; j++)
        {
            uint32_t pos = TO_C_CONT(A, i, j);
            TEST_ASSERT_EQUAL_FLOAT(C->val[pos], 2U * pos);
        }
    }

    /* Wrong sizes */
    MATRIX *D = push_matrix(2U, 2U);
    LOG_DEBUG_MATRIX(C);
    LOG_DEBUG_MATRIX(D);
    LOG_DEBUG("E = C + D");
    MATRIX *E = add(C, D);
    TEST_ASSERT_NULL(E);
}

void test_sub(void)
{
    log_info(__FUNCTION__);

    MATRIX *A = push_matrix(5U, 3U);
    TEST_ASSERT_NOT_NULL(A);

    MATRIX *B = push_matrix(5U, 3U);
    TEST_ASSERT_NOT_NULL(B);

    for (uint32_t i = 0; i < A->rows; i++)
    {
        for (uint32_t j = 0; j < A->cols; j++)
        {
            uint32_t pos = TO_C_CONT(A, i, j);
            A->val[pos] = B->val[pos] = pos;
        }
    }

    /* Wrong input */
    MATRIX *C = sub(A, NULL);
    TEST_ASSERT_NULL(C);

    C = sub(NULL, B);
    TEST_ASSERT_NULL(C);

    C = sub(NULL, NULL);
    TEST_ASSERT_NULL(C);

    LOG_DEBUG_MATRIX(A);
    LOG_DEBUG_MATRIX(B);
    LOG_DEBUG("C = A - B");
    C = sub(A, B);
    TEST_ASSERT_NOT_NULL(C);
    LOG_DEBUG_MATRIX(C);

    for (uint32_t i = 0; i < A->rows; i++)
    {
        for (uint32_t j = 0; j < A->cols; j++)
        {
            uint32_t pos = TO_C_CONT(A, i, j);
            TEST_ASSERT_EQUAL_FLOAT(C->val[pos], 0.0F);
        }
    }

    /* Wrong sizes */
    MATRIX *D = push_matrix(2U, 2U);
    LOG_DEBUG_MATRIX(B);
    LOG_DEBUG_MATRIX(D);
    LOG_DEBUG("E = B - D");
    MATRIX *E = sub(B, D);
    TEST_ASSERT_NULL(E);
}

void test_mult(void)
{
    MATRIX *A = push_matrix(4U, 1U);
    log_info(__FUNCTION__);

    TEST_ASSERT_NOT_NULL(A);

    for (uint32_t i = 0; i < A->rows; i++)
    {
        A->val[i] = 1.0F;
    }

    MATRIX *AT = transpose(A);
    TEST_ASSERT_NOT_NULL(AT);

    /* Wrong input */
    MATRIX *ATA = mult(AT, NULL);
    TEST_ASSERT_NULL(ATA);

    ATA = mult(NULL, A);
    TEST_ASSERT_NULL(ATA);

    ATA = sub(NULL, NULL);
    TEST_ASSERT_NULL(ATA);

    /* dot product */
    LOG_DEBUG_MATRIX(A);
    LOG_DEBUG_MATRIX(AT);
    LOG_DEBUG("ATA = AT * A");
    ATA = mult(AT,A);
    TEST_ASSERT_NOT_NULL(ATA);
    LOG_DEBUG_MATRIX(ATA);
    TEST_ASSERT_EQUAL_UINT32(1U, ATA->rows);
    TEST_ASSERT_EQUAL_UINT32(1U, ATA->cols);
    TEST_ASSERT_EQUAL_FLOAT(4.0F, ATA->val[0]);

    A = push_matrix(3U, 7U);
    MATRIX *B = push_matrix(7U, 4U);

    for (uint32_t i = 0U; i < A->rows * A->cols; i++)
    {
        A->val[i] = 1.0F;
    }

    for (uint32_t i = 0U; i < B->rows * B->cols; i++)
    {
        B->val[i] = 2.0F;
    }

    LOG_DEBUG_MATRIX(A);
    LOG_DEBUG_MATRIX(B);
    LOG_DEBUG("C = A * B");
    MATRIX *C = mult(A, B);
    LOG_DEBUG_MATRIX(C);
    TEST_ASSERT_EQUAL_UINT32(3U, C->rows);
    TEST_ASSERT_EQUAL_UINT32(4U, C->cols);

    for (uint32_t i = 0U; i < A->rows; i++)
    {
        for (uint32_t j = 0U; j < B->cols; j++)
        {
            TEST_ASSERT_EQUAL_FLOAT(14.0F, C->val[TO_C_CONT(C, i, j)]);
        }
    }

    MATRIX *D = push_matrix(2U, 2U);
    LOG_DEBUG_MATRIX(D);
    LOG_DEBUG_MATRIX(C);
    LOG_DEBUG("E = D * C");
    MATRIX *E = mult(D, C);
    TEST_ASSERT_NULL(E);
}

void test_id(void)
{
    /* TODO: to improve push when size is 0U */
    MATRIX *A = id(0U);
    log_info(__FUNCTION__);
    LOG_DEBUG_MATRIX(A);

    TEST_ASSERT_NOT_NULL(A);

    A = id(4U);
    LOG_DEBUG_MATRIX(A);
    TEST_ASSERT_NOT_NULL(A);

    for (uint32_t i = 0U; i < A->rows; i++)
    {
        for (uint32_t j = 0U; j < A->cols; j++)
        {
            uint32_t pos = TO_C_CONT(A, i, j);
            if (i == j)
            {
                TEST_ASSERT_EQUAL_FLOAT(1.0F, A->val[pos]);
            }
            else
            {
                TEST_ASSERT_EQUAL_FLOAT(0.0F, A->val[pos]);
            }
        }
    }
}

void test_permute(void)
{
    MATRIX *I = NULL;
    log_info(__FUNCTION__);

    float expI[25U] = {0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 1.0000000F,
                       0.0000000F, 1.0000000F, 0.0000000F, 0.0000000F, 0.0000000F,
                       0.0000000F, 0.0000000F, 1.0000000F, 0.0000000F, 0.0000000F,
                       0.0000000F, 0.0000000F, 0.0000000F, 1.0000000F, 0.0000000F,
                       1.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F};

    /* wrong arguments */
    I = permute(I, 2U, 5U);
    TEST_ASSERT_NULL(I);
    I = id(5U);
    I = permute(I, 2U, 5U);
    TEST_ASSERT_NOT_NULL(I);
    I = permute(I, 2U, 2U);
    TEST_ASSERT_NOT_NULL(I);

    LOG_DEBUG_MATRIX(I);
    permute(I, 4U, 0U);
    LOG_DEBUG_MATRIX(I);
    for (uint32_t i = 0U; i < I->rows * I->cols; i++)
    {
        TEST_ASSERT_EQUAL_FLOAT(expI[i], I->val[i]);
    }
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_transpose);
    RUN_TEST(test_add);
    RUN_TEST(test_sub);
    RUN_TEST(test_mult);
    RUN_TEST(test_id);
    RUN_TEST(test_permute);

    return UNITY_END();
}
