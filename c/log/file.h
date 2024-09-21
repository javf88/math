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

#include <sys/stat.h>
#include <errno.h>
#include <time.h>

#include "levels.h"

/******************************************************************************/
/*    DEFINITIONS                                                             */
/******************************************************************************/

/* Using C-attribute to force init(c'tor) and shutdown(d'tor) of a file. */
#ifdef LOG_TO_FILE
    #define LOG_CONSTRUCTOR __attribute__((constructor))
#else
    #define LOG_CONSTRUCTOR
#endif

#ifdef LOG_TO_FILE
    #define LOG_DESTRUCTOR __attribute__((destructor))
#else
    #define LOG_DESTRUCTOR
#endif

/******************************************************************************/
/*    API                                                                     */
/******************************************************************************/

#ifdef LOG_UNIT_TEST
    #define CONSTRUCTOR
    #define DESTRUCTOR
#else
    #define CONSTRUCTOR    LOG_CONSTRUCTOR
    #define DESTRUCTOR     LOG_DESTRUCTOR
#endif

/**
 * @brief   Function that is called before main(), and it should NOT be
 *          called explicitly
 */
CONSTRUCTOR void constructor(void);

/**
 * @brief   Function that is called after main(), and it should NOY be
 *          called explicitly
 */
DESTRUCTOR void destructor(void);

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
/*    STATIC FUNCTION PROTOTYPES                                              */
/******************************************************************************/

static LOG* init(LOG *files, uint32_t toFileFlag);

static int32_t make(const char *dir);

static int32_t name(LOG *file);

static LOG* open(LOG *file);

/******************************************************************************/
/*    IMPLEMENTATION                                                          */
/******************************************************************************/

void constructor(void)
{
    int32_t exists;
    log_print(LOG_LEVEL_DEBUG, __FILE__, __LINE__, "Calling constructor().");

    file = init(file, LOG_TO_FILE);
    exists = name(file);
    if (exists > -1)
    {
        open(file);
        /* from levels.h */
        toFile = file->descriptor;
    }

    return;
}

void destructor(void)
{
    /* from levels.h */
    toFile = NULL;

    log_print(LOG_LEVEL_DEBUG, __FILE__, __LINE__, "Calling destructor().");
    if (file != NULL)
    {
        for (LOG *itr = file; itr->name != NULL; itr++)
        {
            log_print(LOG_LEVEL_INFO, __FILE__, __LINE__, "Closing file %s", itr->name);
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


static LOG* init(LOG *files, uint32_t toFileFlag)
{
    const LOG vec[2U] = {{stderr, "stderr"}, {NULL, NULL}};

    if (files != NULL)
    {
        log_print(LOG_LEVEL_WARNING, __FILE__, __LINE__, "Logging module was not initialized.");
        return files;
    }

    /* Creatign a vector of size 2 or 3  */
    files = (LOG*)malloc(sizeof(LOG) * 2U + toFileFlag);
    memcpy(&files[toFileFlag], vec, sizeof(LOG) * 2U);

    return files;
}

static int32_t make(const char *dir)
{
    /* Creating rel dir */
    mkdir(dir, 0777);
    switch (errno)
    {
        case 0:
            log_print(LOG_LEVEL_INFO, __FILE__, __LINE__, "Directory %s was created. [CODE %d]", dir, errno);
            break;
        case EEXIST:
            log_print(LOG_LEVEL_WARNING, __FILE__, __LINE__, "Directory %s exists already. [CODE %d]", dir, errno);
            break;
        case ENOENT:
        default:
            /* If dir is not created, opening file routine cleans up */
            log_print(LOG_LEVEL_WARNING, __FILE__, __LINE__, "Directory %s was not created! [CODE %d]", dir, errno);
            break;
    }

    return errno;
}

static int32_t name(LOG *file)
{
    int32_t ret;

    file->name = malloc(sizeof(char) * MAX_STR_LEN);
    if (file->name == NULL)
    {
        log_print(LOG_LEVEL_WARNING, __FILE__, __LINE__, "Logging file was not created.");
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

static LOG* open(LOG *file)
{
    LOG *ret = NULL;
    file->descriptor = fopen(file->name, "w");
    if (file->descriptor == NULL)
    {
        log_print(LOG_LEVEL_WARNING, __FILE__, __LINE__, "File was not created! [CODE %d]", errno);
        free(file->name);
        free(file);

        ret = NULL;
    }
    else
    {
        log_print(LOG_LEVEL_INFO, __FILE__, __LINE__, "File %s was created.", file->name);
        ret = file;
    }

    return ret;
}

#ifdef __cplusplus
}
#endif

#endif /* FILE_H_ */
