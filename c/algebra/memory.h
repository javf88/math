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

#include "levels.h"
#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

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
/*    DEFINITIONS                                                             */
/******************************************************************************/

/* Mapping from a matrix, A[i, j], to C-contiguous layout */
#define TO_C_CONT(A, row, col) A->cols * row + col

/* Mapping from a matrix, AT[i, j], to Fortran-contiguous layout */
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

void* get_block_matrix(MATRIX *S, uint32_t row, uint32_t rowEnd,
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
