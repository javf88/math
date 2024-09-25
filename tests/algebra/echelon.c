/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

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

void test_get_new_pivot(void)
{
    MATRIX *B = push_matrix(5U, 5U);
    float vals[25U] = {-4.0000000F, -2.0000000F, 0.0000000F, 0.0000000F, 0.0000000F,
                        0.0000000F,  0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F,
                        0.0000000F,  0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F,
                        0.0000000F,  1.0000000F, 0.0000000F, 0.0000000F, 0.0000000F,
                 2.0F * FLT_EPSILON, 0.0000000F, 0.0000000F, 1.0000000F, 0.0000000F};
    uint32_t expRow[5U] = {4U, 2U, 3U, 1U, 1U};
    memcpy(B->val, vals, sizeof(float) * B->rows * B->cols);

    LOG_INFO("%s", __func__);
    for (uint32_t i = 0U; i < B->rows; i++)
    {
        MATRIX *A = GET_BLOCK_MATRIX(B, i);
        uint32_t row = get_new_pivot(A);
        LOG_DEBUG("row %u", row);
        LOG_DEBUG_MATRIX(A);
        TEST_ASSERT_EQUAL_UINT32(expRow[i], row);
    }
}

void test_get_permutation(void)
{
    MATRIX *B = push_matrix(5U, 5U);
    float vals[25U] = {-4.0000000F, -2.0000000F, 0.0000000F, 0.0000000F, 0.0000000F,
                        0.0000000F,  0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F,
                        0.0000000F,  0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F,
                        0.0000000F,  1.0000000F, 0.0000000F, 0.0000000F, 0.0000000F,
                 2.0F * FLT_EPSILON, 0.0000000F, 0.0000000F, 1.0000000F, 0.0000000F};
    void *expP[5U] = {(void*)1U, (void*)1U, NULL, (void*)1U, NULL};
    memcpy(B->val, vals, sizeof(float) * B->rows * B->cols);

    LOG_INFO("%s", __func__);
    for (uint32_t i = 0U; i < B->rows; i++)
    {
        MATRIX *A = GET_BLOCK_MATRIX(B, i);
        MATRIX *P = get_permutation(A);
        LOG_DEBUG("row %u", i);
        if (expP[i] != NULL)
        {
            TEST_ASSERT_NOT_NULL(P);
            LOG_DEBUG_MATRIX(A);
            LOG_DEBUG_MATRIX(P);
        }
        else
        {
            TEST_ASSERT_NULL(P);
        }
    }
}

void test_get_lower_triangular(void)
{
    MATRIX *PA = push_matrix(4U, 4U);
    float vals[16U] = {-50.0000000F, 0.0000000F, 16.6666679F, 25.0000000F,
                       30.0000019F, 33.3333359F, 35.7142868F, 37.5000000F,
                       38.8888893F, 40.0000000F, 40.9090919F, 41.6666641F,
                       42.3076935F, 42.8571434F, 43.3333321F, 43.7500000F};
    MATRIX *LVecs = push_matrix(4U, 4U);
    float expLs[16U] = {1.0000000F,  0.0000000F,  0.0000000F, 0.0000000F,
                        0.6000000F,  1.0000000F,  0.0000000F, 0.0000000F,
                        0.7777778F, -1.1999999F,  1.0000000F, 0.0000000F,
                        0.8461539F, -1.2857141F, -1.3598906F, -1.0000000F};
    memcpy(PA->val, vals, sizeof(float) * PA->rows * PA->cols);
    memcpy(LVecs->val, expLs, sizeof(float) * LVecs->rows * LVecs->cols);

    LOG_INFO("%s", __func__);
    for (uint32_t i = 0U; i < LVecs->rows; i++)
    {
        MATRIX *L;
        LOG_DEBUG_MATRIX(PA);
        L = get_lower_triangular(PA);
        LOG_DEBUG_MATRIX(L);
        LOG_DEBUG_MATRIX(LVecs);
        for (uint32_t j = i; j < LVecs->rows; j++)
        {
            TEST_ASSERT_EQUAL_FLOAT(expLs[TO_C_CONT(LVecs, j, i)], L->val[TO_C_CONT(L, (j - i), 0U)]);
        }
        PA = mult(L, PA);
        LOG_DEBUG_MATRIX(PA);
        PA = GET_BLOCK_MATRIX(PA, 1U);
    }
}

void test_echelon_rect_matrix(void)
{
    MATRIX *A = push_matrix(2U, 3U);

    LOG_INFO("%s", __func__);
    /* Wrong input */
    TEST_ASSERT_NULL(echelon(A));

    do {
        /* The stack starts with a non-NULL value */
        TEST_ASSERT_NOT_NULL(stack);
        stack = pop_matrix(stack);
    } while(stack != NULL);
}

void test_echelon_perfect_matrix(void)
{
    MATRIX *A = push_matrix(4U, 4U);
    float vals[16U] = {-50.0000000F, 0.0000000F, 16.6666679F, 25.0000000F,
                       30.0000019F, 33.3333359F, 35.7142868F, 37.5000000F,
                       38.8888893F, 40.0000000F, 40.9090919F, 41.6666641F,
                       42.3076935F, 42.8571434F, 43.3333321F, 43.7500000F};
    float expected[16U] = {-50.0000000F, 0.0000000F, 16.6666679F, 25.0000000F,
                            0.0000000F, 33.3333359F, 45.7142868F, 52.5000000F,
                             0.0000000F, 0.0000000F, -0.9850845F, -1.8888893F,
                             0.0000000F, 0.0000000F, 0.0000000F, -0.0274627F};
    memcpy(A->val, vals, sizeof(float) * A->rows * A->cols);

    LOG_INFO_MATRIX(A);
    A = echelon(A);
    LOG_INFO_MATRIX(A);

    for (uint32_t i = 0U; i < 4U * 4U; i++)
    {
        TEST_ASSERT_EQUAL_FLOAT(expected[i], A->val[i]);
    }
}

void test_echelon_singular_matrix(void)
{
    MATRIX *A = push_matrix(5U, 5U);
    float singular[25U]  = {-12.0000000F, -9.5000000F, -7.0000000F, -4.5000000F, -2.0000000F,
                               0.5000000F, 3.0000000F, 5.5000000F, 8.0000000F, 10.5000000F,
                             13.0000000F, 15.5000000F, 18.0000000F, 20.5000000F, 23.0000000F,
                             25.5000000F, 28.0000000F, 30.5000000F, 33.0000000F, 35.5000000F,
                             38.0000000F, 40.5000000F, 43.0000000F, 45.5000000F, 48.0000000F};
    memcpy(A->val, singular, sizeof(float) * 5U * 5U);

    LOG_INFO_MATRIX(A);
    A = echelon(A);
    LOG_INFO_MATRIX(A);
    float expA[25U] = {-12.0000000F, -9.5000000F, -7.0000000F, -4.5000000F, -2.0000000F,
                          0.0000000F, 2.6041667F, 5.2083335F, 7.8125000F, 10.4166670F,
                          0.0000000F, 0.0000000F, 0.0000000F, 0.000001907349F, 0.000001907349F,
                          0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F,
                          0.0000000F, 0.0000000F, 0.0000000F, 0.00000381469F, 0.00000381469F};

    for (uint32_t i = 0; i < 5U * 5U; i++)
    {
        TEST_ASSERT_EQUAL_FLOAT(expA[i], A->val[i]);
    }

    do {
        /* The stack starts with a non-NULL value */
        TEST_ASSERT_NOT_NULL(stack);
        stack = pop_matrix(stack);
    } while(stack != NULL);
}

void test_echelon_only_permutations(void)
{
    MATRIX *A = push_matrix(5U, 5U);
    float val[25U] = {0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 1.0000000F,
                      0.0000000F, 0.0000000F, 0.0000000F, 2.0000000F, 2.0000000F,
                      0.0000000F, 0.0000000F, 3.0000000F, 3.0000000F, 3.0000000F,
                      0.0000000F, 4.0000000F, 4.0000000F, 4.0000000F, 4.0000000F,
                      5.0000000F, 5.0000000F, 5.0000000F, 5.0000000F, 5.0000000F};
    float expVal[25U] = {5.0000000F, 5.0000000F, 5.0000000F, 5.0000000F, 5.0000000F,
                         0.0000000F, 4.0000000F, 4.0000000F, 4.0000000F, 4.0000000F,
                         0.0000000F, 0.0000000F, 3.0000000F, 3.0000000F, 3.0000000F,
                         0.0000000F, 0.0000000F, 0.0000000F, 2.0000000F, 2.0000000F,
                         0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 1.0000000F};
    memcpy(A->val, val, sizeof(float) * A->rows * A->cols);

    LOG_INFO("%s", __func__);
    LOG_INFO_MATRIX(A);
    A = echelon(A);
    LOG_INFO_MATRIX(A);

    for (uint32_t i = 0; i < A->rows * A->cols; i++)
    {
        TEST_ASSERT_EQUAL_FLOAT(expVal[i], A->val[i]);
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

    RUN_TEST(test_get_new_pivot);
    RUN_TEST(test_get_permutation);
    RUN_TEST(test_get_lower_triangular);
    RUN_TEST(test_echelon_rect_matrix);
    RUN_TEST(test_echelon_only_permutations);
//    RUN_TEST(test_echelon_perfect_matrix);
//    RUN_TEST(test_echelon_singular_matrix);

    return UNITY_END();
}
