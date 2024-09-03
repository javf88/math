#include <stdio.h>
#include <stdint.h>

enum ContiguousFlag
{
    C_CONTIGUOUS,
    FORTRAN_CONTIGUOUS
};

typedef struct Matrix
{
    uint32_t contiguousFlag;
    uint32_t rows;
    uint32_t cols;
    float   *a;
} MATRIX;


/* I still need to figure out how to deal with buffers later! */
uint8_t buffer[sizeof(float) * 256U];

uint32_t init(float *buffer)
{
    for (uint32_t i = 0; i < 256U; i++)
    {
        buffer[i] = (float)i;
    }

    return 1U;
}

int main()
{
    printf("Initial commit\n");

    MATRIX A = {.contiguousFlag = C_CONTIGUOUS,
                .rows = 1U,
                .cols = 3U,
                .a = (float*)&buffer};

    MATRIX B = {.contiguousFlag = C_CONTIGUOUS,
                .rows = 1U,
                .cols = 3U,
                .a = (float*)&buffer};

    return 0;
}
