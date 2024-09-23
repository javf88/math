/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include "levels.h"
#include "unity.h"
/* TARGET LIBRARY */
#include "echelon.h"

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

    /* not pivot in 2nd row*/
    newRow = get_row(A, 2U);
    TEST_ASSERT_EQUAL_UINT32(A->rows, newRow);

    newRow = get_row(A, 3U);
    TEST_ASSERT_EQUAL_UINT32(4U, newRow);
}

void test_update(void)
{
    MATRIX *A = push_matrix(5U, 5U);
    float val[25U] = {1.0000000F, 1.0000000F, 1.0000000F, 1.0000000F, 1.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 5.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 4.0000000F, 4.0000000F, 0.0000000F, 2.0000000F, 2.0000000F, 2.0000000F, 2.0000000F, 0.0000000F, 0.0000000F, 3.0000000F, 3.0000000F, 3.0000000F};
    float expVal[25U] = {1.0000000F, 1.0000000F, 1.0000000F, 1.0000000F, 1.0000000F, 0.0000000F, 2.0000000F, 2.0000000F, 2.0000000F, 2.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 4.0000000F, 4.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 5.0000000F, 0.0000000F, 0.0000000F, 3.0000000F, 3.0000000F, 3.0000000F};
    float expVal2[25U] = {1.0000000F, 1.0000000F, 1.0000000F, 1.0000000F, 1.0000000F, 0.0000000F, 2.0000000F, 2.0000000F, 2.0000000F, 2.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 4.0000000F, 4.0000000F, 0.0000000F, 0.0000000F, 3.0000000F, 3.0000000F, 3.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 5.0000000F};
    memcpy(A->val, val, sizeof(float) * 5U * 5U);

    LOG_INFO_MATRIX(A);
    A = update(A, 1U);
    for (uint32_t i = 0U; i < A->rows * A->cols; i++)
    {
        TEST_ASSERT_EQUAL_FLOAT(expVal[i], A->val[i]);
    }

    A = update(A, 3U);
    for (uint32_t i = 0U; i < A->rows * A->cols; i++)
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

    /* Wrong input */
    MATRIX *B = echelon(A);
    TEST_ASSERT_NULL(B);

    B = push_matrix(4U, 4U);
    float vals[16U] = {-50.0000000F, 0.0000000F, 16.6666679F, 25.0000000F, 30.0000019F, 33.3333359F, 35.7142868F, 37.5000000F, 38.8888893F, 40.0000000F, 40.9090919F, 41.6666641F, 42.3076935F, 42.8571434F, 43.3333321F, 43.7500000F};
    float expected[16U] = {-50.0000000F, 0.0000000F, 16.6666679F, 25.0000000F, 0.0000000F, 33.3333359F, 45.7142868F, 52.5000000F, 0.0000000F, 0.0000000F, -0.9850845F, -1.8888893F, 0.0000000F, 0.0000000F, 0.0000000F, -0.0274627F};
    memcpy(B->val, vals, sizeof(float) * B->rows * B->cols);

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

    RUN_TEST(test_get_row);
    RUN_TEST(test_update);
    RUN_TEST(test_echelon);

    return UNITY_END();
}
