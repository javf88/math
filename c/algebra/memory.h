/*******************************************************************************
*
* MEMORY MANAGEMENT SYSTEM
*
*   SUMMARY
*       This single-header module implements a simple but effective memory
*       management system, the M2S. Its functionality is boiled down to:
*
*       a) matrix creation, and
*       b) matrix destruction.
*
*******************************************************************************/

#ifndef MEMORY_H_
#define MEMORY_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/******************************************************************************/
/*    PUBLIC TYPES                                                            */
/******************************************************************************/

/* A simple abstraction for Matrix */
typedef struct Matrix
{
    uint32_t rows;
    uint32_t cols;
    float   *val;
} MATRIX;

/******************************************************************************/
/*    PUBLIC MACROS                                                           */
/******************************************************************************/

/**
 * @brief   Macro to get a row-vector from a matrix.
 */
#define GET_ROW_VECTOR(matrix, row) \
    get_block_matrix(matrix, row, row + 1U, 0U, matrix->cols)

/**
 * @brief   Macro to get a column-vector from a matrix.
 */
#define GET_COLUMN_VECTOR(matrix, col) \
    get_block_matrix(matrix, 0U, matrix->rows, col, col + 1U)

/**
 * @brief   Macro to get a lower-right block from a matrix.
 */
#define GET_BLOCK_MATRIX(matrix, i) \
    get_block_matrix(matrix, i, matrix->rows, i, matrix->cols)

/**
 * @brief   Macro to copy a matrix.
 */
#define COPY_MATRIX(matrix) \
    get_block_matrix(matrix, 0U, matrix->rows, 0U, matrix->cols)

/**
 * @brief   Macro to map an entry A(i,j) to its c-contiguous.
 */
#define TO_C_CONT(A, row, col) A->cols * row + col

/**
 * @brief   Macro to map an entry A(i,j) to its fortran-contiguous.
 */
#define TO_F_CONT(A, row, col) A->rows * col + row

/******************************************************************************/
/*    API                                                                     */
/******************************************************************************/

/**
 * @brief   Function that creates a MATRIX and pushes it on the stack
 */
MATRIX* push_matrix(uint32_t rows, uint32_t cols);

/**
 * @brief   Function that deletes a MATRIX and pops it from the stack.
 *          void is used because ITEM is defined in the private data
 */
void* pop_matrix(void *top);

/**
 * @brief   Function that get a block-matrix from a matrix, it takes two
 *          points (row, col) and (rowEnd, colEnd).
 */
MATRIX* get_block_matrix(MATRIX *S, uint32_t row, uint32_t rowEnd,
                                  uint32_t col, uint32_t colEnd);

/**
 * @brief   Function that set a block-matrix in a matrix D, it takes one
 *          point (row, col), and it sets matrix S if it fits.
 */
MATRIX* set_block_matrix(MATRIX *D, uint32_t row, uint32_t col, MATRIX *S);

/******************************************************************************/
/*    PRIVATE DATA                                                            */
/******************************************************************************/

/* A simple stack */
typedef struct Item
{
    MATRIX *matrix;
    struct Item *next;
} ITEM;

static ITEM *stack = NULL;

/******************************************************************************/
/*    STATIC FUNCTION PROTOTYPES                                              */
/******************************************************************************/

static MATRIX* matrix_malloc(uint32_t rows, uint32_t cols);

static ITEM* matrix_push(ITEM *stack, MATRIX *A);

static MATRIX* matrix_free(MATRIX *matrix);

/******************************************************************************/
/*    IMPLEMENTATION                                                          */
/******************************************************************************/

MATRIX* push_matrix(uint32_t rows, uint32_t cols)
{
    MATRIX *A = matrix_malloc(rows, cols);

    if (A == NULL)
    {
        return NULL;
    }
    else
    {
        stack = matrix_push(stack, A);
    }

    return A;
}

/* Freeing only one item from the stack */
void* pop_matrix(void *top)
{
    if (top != NULL)
    {
        ITEM *tmp = ((ITEM*)top)->next;
        matrix_free(((ITEM*)top)->matrix);
        free(top);
        top = tmp;
    }

    return top;
}

MATRIX* get_block_matrix(MATRIX *S, uint32_t row, uint32_t rowEnd,
                                  uint32_t col, uint32_t colEnd)
{
    MATRIX *block = NULL;

    if (S == NULL)
    {
        return NULL;
    }

    /* row < rowEnd <= S->rows, the same holds for cols, to be a valid call */
    if ((rowEnd <= row) || (colEnd <= col) ||
        (S->rows < rowEnd) || (S->cols < colEnd))
    {
        return S;
    }

    block = push_matrix(rowEnd - row, colEnd - col);
    for (uint32_t i = row; i < rowEnd; i++)
    {
        uint32_t srcIdx = TO_C_CONT(S, i, col);
        uint32_t dstIdx = TO_C_CONT(block, (i - row), 0U);
        memcpy(&block->val[dstIdx], &S->val[srcIdx], sizeof(float) * block->cols);
    }

    return block;
}

MATRIX* set_block_matrix(MATRIX *D, uint32_t row, uint32_t col, MATRIX *S)
{
    if ((D == NULL) || (S == NULL))
    {
        return NULL;
    }

    if ((D->rows < row) || (D->cols < col))
    {
        return D;
    }

    if ((D->rows - row) < (S->rows) ||
        (D->cols - col) < (S->cols))
    {
        return S;
    }

    for (uint32_t i = 0U; i < S->rows; i++)
    {
        uint32_t dstIdx = TO_C_CONT(D, (i + row), col);
        uint32_t srcIdx = TO_C_CONT(S, i, 0U);
        memcpy(&D->val[dstIdx], &S->val[srcIdx], sizeof(float) * S->cols);
    }

    return D;
}

/* Memory allocation-like functions */
static MATRIX* matrix_malloc(uint32_t rows, uint32_t cols)
{
    MATRIX *matrix = (MATRIX*)malloc(sizeof(MATRIX));

    if (matrix != NULL)
    {
        matrix->rows = rows;
        matrix->cols = cols;
        matrix->val = (float*)malloc(sizeof(float) * rows * cols);
    }

    return matrix;
}

static ITEM* matrix_push(ITEM *stack, MATRIX *A)
{
    ITEM *top = (ITEM*)malloc(sizeof(ITEM));

    /* stack is NULL at the very first itr */
    top->next = stack;
    top->matrix = A;

    return top;
}

/* Memory free-like functions */
static MATRIX* matrix_free(MATRIX *matrix)
{
    if (matrix->val != NULL)
    {
        free(matrix->val);
    }

    if (matrix != NULL)
    {
        free(matrix);
    }

    return (MATRIX*)NULL;
}

#ifdef __cplusplus
}
#endif

#endif /* MEMORY_H_ */
