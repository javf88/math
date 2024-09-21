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
#include <time.h>

#include "levels.h"

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

#define MAX_NAME_LEN  (50U)

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

int32_t name(LOG *file)
{
    int32_t ret;

    file->name = malloc(sizeof(char) * MAX_STR_LEN);
    if (file->name == NULL)
    {
        LOG_WARNING("Logging file was not created.");
        ret = -1;
    }
    else
    {
        /* Creating dir */
        ret = make("tmp");
        if (ret > -1)
        {
            time_t secs;
            /* Creating tmp name */
            time(&secs);
            memset(file->name, 0, sizeof(char) * MAX_NAME_LEN);
            sprintf(file->name, "tmp/%ld.log", secs);
        }
    }

    return ret;
}

LOG* open(LOG *file)
{
    LOG *ret = NULL;
    file->descriptor = fopen(file->name, "w");
    if (file->descriptor == NULL)
    {
        LOG_WARNING("File was not created! [CODE %d]", errno);
        free(file->name);
        free(file);

        ret = NULL;
    }
    else
    {
        LOG_INFO("File %s was created.", file->name);
        ret = file;
    }

    return ret;
}

void constructor(void)
{
    int32_t exists;

    file = init(file, LOG_TO_FILE);
    exists = name(file);
    if (exists > -1)
    {
        open(file);
    }

    return;
}

void destructor(void)
{
    if (file != NULL)
    {
        for (LOG *itr = file; itr->name != NULL; itr++)
        {
            printf("Closing file %s, %p\n", itr->name, itr->name);
            fflush(itr->descriptor);
            fclose(itr->descriptor);
            if (strcmp(itr->name, "stderr") != 0)
            {
                free(itr->name);
                itr->descriptor = NULL;
                itr->name = NULL;
            }
        }
    }

    file = NULL;

    return;
}

#ifdef __cplusplus
}
#endif

#endif /* FILE_H_ */
