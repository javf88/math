/*******************************************************************************
*
* LOGGING SYSTEM
*
*   SUMMARY
*
*******************************************************************************/

#ifndef FILE_H_
#define FILE_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <errno.h>

/******************************************************************************/
/*    DEFINITIONS                                                             */
/******************************************************************************/


/******************************************************************************/
/*    PUBLIC FUNCTION-LIKE MACRO                                             */
/******************************************************************************/


/******************************************************************************/
/*    PRIVATE DATA                                                            */
/******************************************************************************/

typedef struct Log
{
    FILE *descriptor;
    char *name;
} LOG;

/* file handler */
static LOG *file = NULL;

/******************************************************************************/
/*    IMPLEMENTATION                                                          */
/******************************************************************************/

LOG* init(LOG *files, uint32_t toFileFlag)
{
    const LOG vec[2U] = {{stderr, "stderr"}, {NULL, NULL}};

    if (files != NULL)
    {
        fprintf(stderr, "Logging module was not initialized.\n");
        return files;
    }

    files = (LOG*)malloc(sizeof(LOG) * 2U + toFileFlag);
    memcpy(&files[toFileFlag], vec, sizeof(LOG) * 2U);

    return files;
}

int32_t make(const char *dir)
{
    /* Creating rel dir */
    mkdir(dir, 0777);
    switch (errno)
    {
        case 0:
            fprintf(stderr,"Directory %s was created. [CODE %d]", dir, errno);
            break;
        case EEXIST:
            fprintf(stderr,"Directory %s exists already. [CODE %d]", dir, errno);
            break;
        case ENOENT:
        default:
            /* If dir is not created, opening file routine cleans up */
            fprintf(stderr,"Directory %s was not created! [CODE %d]", dir, errno);
            break;
    }

    return errno;
}

#ifdef __cplusplus
}
#endif

#endif /* FILE_H_ */
