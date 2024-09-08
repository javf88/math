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
/*    PUBLIC FUNCTIONS                                                        */
/******************************************************************************/

/* Matrix creation, push_matrix:
 *  - allocates the memory for a matrix, whose dimensions are rows x cols;
 *  - and pushes the matrix on the stack. */
MATRIX* push_matrix(uint32_t rows, uint32_t cols);

/* Matrix destruction, pop_matrix:
 *  - frees the memory of a matrix;
 *  - and pops the top item of the stack.
 *
 *  void is used because ITEM is defined in the private data */
void* pop_matrix(void *top);

/******************************************************************************/
/*    PRIVATE DATA AND IMPLEMENTATION                                         */
/******************************************************************************/

/* PRIVATE DATA */

/* A simple stack */
typedef struct Item
{
    MATRIX *matrix;
    struct Item *next;
} ITEM;

static ITEM *stack = NULL;

/* PRIVATE FUNCTION PROTOTYPES */

static MATRIX* _M2S_malloc(uint32_t rows, uint32_t cols);

static ITEM* _M2S_push(ITEM *stack, MATRIX *A);

static MATRIX* _M2S_free(MATRIX *matrix);

/* Memory allocation-like functions */
static MATRIX* _M2S_malloc(uint32_t rows, uint32_t cols)
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

static ITEM* _M2S_push(ITEM *stack, MATRIX *A)
{
    ITEM *top = (ITEM*)malloc(sizeof(ITEM));

    /* stack is NULL at the very first itr */
    top->next = stack;
    top->matrix = A;

    return top;
}

/* Memory free-like functions */
static MATRIX* _M2S_free(MATRIX *matrix)
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

/* PUBLIC FUNCTIONS */

MATRIX* push_matrix(uint32_t rows, uint32_t cols)
{
    MATRIX *A = _M2S_malloc(rows, cols);

    if (A == NULL)
    {
        return NULL;
    }
    else
    {
        stack = _M2S_push(stack, A);
    }

    return A;
}

/* Freeing only one item from the stack */
void* pop_matrix(void *top)
{
    /* Creating a temporal variable, to avoid casting as ((ITEM*)top) */
    ITEM* _top = top;
    if (_top != NULL)
    {
        ITEM *tmp = _top->next;
        _M2S_free(_top->matrix);
        free(_top);
        _top = tmp;
    }

    return _top;
}

#ifdef __cplusplus
}
#endif

#endif /* MEMORY_H_ */
