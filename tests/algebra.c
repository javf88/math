/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include "unity.h"
/* TARGET LIBRARY */
#include "algebra.h"

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

    MATRIX *C = NULL;
    C = transpose(C);
    TEST_ASSERT_NULL(C);
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

void test_get_row(void)
{
    float vals[25U] = {-4.0000000F, -2.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F,  0.0000000F,0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 1.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 2.0F * FLT_EPSILON, 0.0000000F, 0.0000000F, 1.0000000F, 0.0000000F, };
    MATRIX *A = push_matrix(5U, 5U);
    memcpy(A->val, vals, sizeof(float) * 25U);

    LOG_INFO_MATRIX(A);
    uint32_t newRow = get_row(A, 0U);
    TEST_ASSERT_EQUAL_UINT32(4U, newRow);

    newRow = get_row(A, 1U);
    TEST_ASSERT_EQUAL_UINT32(3U, newRow);

    newRow = get_row(A, 2U);
    TEST_ASSERT_EQUAL_UINT32(A->rows, newRow);

    newRow = get_row(A, 3U);
    TEST_ASSERT_EQUAL_UINT32(4U, newRow);
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

void test_update(void)
{
    MATRIX *A = push_matrix(5U, 5U);
    float val[25U] = {1.0000000F, 1.0000000F, 1.0000000F, 1.0000000F, 1.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 5.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 4.0000000F, 4.0000000F, 0.0000000F, 2.0000000F, 2.0000000F, 2.0000000F, 2.0000000F, 0.0000000F, 0.0000000F, 3.0000000F, 3.0000000F, 3.0000000F};
    float expVal[25U] = {1.0000000F, 1.0000000F, 1.0000000F, 1.0000000F, 1.0000000F, 0.0000000F, 2.0000000F, 2.0000000F, 2.0000000F, 2.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 4.0000000F, 4.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 5.0000000F, 0.0000000F, 0.0000000F, 3.0000000F, 3.0000000F, 3.0000000F};
    float expVal2[25U] = {1.0000000F, 1.0000000F, 1.0000000F, 1.0000000F, 1.0000000F, 0.0000000F, 2.0000000F, 2.0000000F, 2.0000000F, 2.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 4.0000000F, 4.0000000F, 0.0000000F, 0.0000000F, 3.0000000F, 3.0000000F, 3.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 5.0000000F};
    memcpy(A->val, val, sizeof(float) * 5U * 5U);

    A = update(A, 1U);
    for (uint32_t i = 0U; i < 5U * 5U; i++)
    {
        TEST_ASSERT_EQUAL_FLOAT(expVal[i], A->val[i]);
    }

    A = update(A, 3U);
    for (uint32_t i = 0U; i < 5U * 5U; i++)
    {
        TEST_ASSERT_EQUAL_FLOAT(expVal2[i], A->val[i]);
    }

    do {
        /* The stack starts with a non-NULL value */
        TEST_ASSERT_NOT_NULL(stack);
        stack = pop_matrix(stack);
    } while(stack != NULL);
}

void test_echelon(void)
{
    MATRIX *A = push_matrix(2U, 3U);
    TEST_ASSERT_NOT_NULL(A);

    MATRIX *B = echelon(A);
    TEST_ASSERT_NULL(B);

    B = push_matrix(4U, 4U);
    float vals[16U] = {-50.0000000F, 0.0000000F, 16.6666679F, 25.0000000F, 30.0000019F, 33.3333359F, 35.7142868F, 37.5000000F, 38.8888893F, 40.0000000F, 40.9090919F, 41.6666641F, 42.3076935F, 42.8571434F, 43.3333321F, 43.7500000F};
    float expected[16U] = {-50.0000000F, 0.0000000F, 16.6666679F, 25.0000000F, 0.0000000F, 33.3333359F, 45.7142868F, 52.5000000F, 0.0000000F, 0.0000000F, -0.9850845F, -1.8888893F, 0.0000000F, 0.0000000F, 0.0000000F, -0.0274627F};
    memcpy(B->val, vals, sizeof(float) * 4U * 4U);

    LOG_INFO_MATRIX(B);
    B = echelon(B);
    LOG_INFO_MATRIX(B);

    for (uint32_t i = 0U; i < 4U * 4U; i++)
    {
        TEST_ASSERT_EQUAL_FLOAT(expected[i], B->val[i]);
    }

    MATRIX *C = push_matrix(5U, 5U);
    float singular[25U]  = {-12.0000000F, -9.5000000F, -7.0000000F, -4.5000000F, -2.0000000F, 0.5000000F, 3.0000000F, 5.5000000F, 8.0000000F, 10.5000000F, 13.0000000F, 15.5000000F, 18.0000000F, 20.5000000F, 23.0000000F, 25.5000000F, 28.0000000F, 30.5000000F, 33.0000000F, 35.5000000F, 38.0000000F, 40.5000000F, 43.0000000F, 45.5000000F, 48.0000000F};
    memcpy(C->val, singular, sizeof(float) * 5U * 5U);

    LOG_INFO_MATRIX(C);
    C = echelon(C);
    LOG_INFO_MATRIX(C);
    float expC[25U] = {-12.0000000F, -9.5000000F, -7.0000000F, -4.5000000F, -2.0000000F, 0.0000000F, 2.6041667F, 5.2083335F, 7.8125000F, 10.4166670F, 0.0000000F, 0.0000000F, 0.0000000F, 0.000001907349F, 0.000001907349F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.00000381469F, 0.00000381469F};

    for (uint32_t i = 0; i < 5U * 5U; i++)
    {
        TEST_ASSERT_EQUAL_FLOAT(expC[i], C->val[i]);
    }

    MATRIX *D = push_matrix(5U, 5U);
    float permutation[25U]  = {-12.0000000F, -9.5000000F, -7.0000000F, -4.5000000F, -2.0000000F, 0.5000000F, 3.0000000F, 5.5000000F, 8.0000000F, 10.5000000F, 13.0000000F, 15.5000000F, 18.0000000F, 20.5000000F, 23.0000000F, 65.5000000F, 28.4000000F, 3.5000000F, 13.0000000F, 35.0000000F, 38.0000000F, 40.5000000F, 43.0000000F, 45.5000000F, 48.0000000F};
    memcpy(D->val, permutation, sizeof(float) * 5U * 5U);
    LOG_INFO_MATRIX(D);
    D = echelon(D);
    LOG_INFO_MATRIX(D);
    float expPerm[25U] = {-12.0000000F, -9.5000000F, -7.0000000F, -4.5000000F, -2.0000000F, 0.0000000F,  2.6041667F,  5.2083335F,  7.8125000F, 10.4166670F, 0.0000000F,  0.0000000F, 12.2000008F, 58.8000031F, 117.9000092F, 0.0000000F,  0.0000000F,  0.0000000F,  0.0000019F,  0.0000019F, 0.0000000F,  0.0000000F,  0.0000000F,  0.0000000F,  0.0000000F};

    for (uint32_t i = 0; i < 5U * 5U; i++)
    {
        TEST_ASSERT_FLOAT_WITHIN(FLT_EPSILON, expPerm[i], D->val[i]);
    }

    MATRIX *E = push_matrix(5U, 5U);
    float val[25U] = {0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 1.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 2.0000000F, 2.0000000F, 0.0000000F, 0.0000000F, 3.0000000F, 3.0000000F, 3.0000000F, 0.0000000F, 4.0000000F, 4.0000000F, 4.0000000F, 4.0000000F, 5.0000000F, 5.0000000F, 5.0000000F, 5.0000000F, 5.0000000F};
    float expVal[25U] = {5.0000000F, 5.0000000F, 5.0000000F, 5.0000000F, 5.0000000F, 0.0000000F, 4.0000000F, 4.0000000F, 4.0000000F, 4.0000000F, 0.0000000F, 0.0000000F, 3.0000000F, 3.0000000F, 3.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 2.0000000F, 2.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 1.0000000F};
    memcpy(E->val, val, sizeof(float) * 5U * 5U);
    LOG_INFO_MATRIX(E);
    E = echelon(E);
    LOG_INFO_MATRIX(E);

    for (uint32_t i = 0; i < 5U * 5U; i++)
    {
        TEST_ASSERT_EQUAL_FLOAT(expVal[i], E->val[i]);
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
    RUN_TEST(test_subs);
    RUN_TEST(test_mult);
    RUN_TEST(test_id);
    RUN_TEST(test_get_row);
    RUN_TEST(test_permute);
    RUN_TEST(test_update);
    RUN_TEST(test_echelon);

    return UNITY_END();
}
