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

/* A function that create a matrix and handles all the memory allocation */
MATRIX* create_matrix(uint32_t rows, uint32_t cols);

/* A function that frees all matrices that have not been individually freed */
void* free_matrices(void *head);

/******************************************************************************/
/*    IMPLEMENTATION AND INTERNALS                                            */
/******************************************************************************/

/* A simple linked list */
typedef struct Node
{
    MATRIX *matrix;
    struct Node *next;
} NODE;

static NODE *list = NULL;

/* Memory allocation-like functions */
MATRIX* _malloc(uint32_t rows, uint32_t cols)
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

NODE* append(NODE *head, MATRIX *A)
{
    NODE *tail = (NODE*)malloc(sizeof(NODE));
    tail->matrix = A;
    /* head is NULL at the very first itr */
    tail->next = head;

    return tail;
}

MATRIX* create_matrix(uint32_t rows, uint32_t cols)
{
    MATRIX *A = _malloc(rows, cols);

    if (A == NULL)
    {
        return NULL;
    }
    else
    {
        /* A bit dirty here */
        list = append(list, A);
    }

    return A;
}

/* Memory free-like functions */
MATRIX* _free(MATRIX *matrix)
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

void* free_matrices(void *list)
{
    NODE *head = list;
    while (head != NULL)
    {
        NODE *tmp = head->next;
        _free(head->matrix);
        free(head);
        head = tmp;
    }

    return head;
}

#ifdef __cplusplus
}
#endif

#endif /* MEMORY_H_ */
