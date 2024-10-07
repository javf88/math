/*******************************************************************************
*
* LOGGING SYSTEM
*
*   SUMMARY - file submodule
*       This single-header submodule implements the logging to file feature
*       of the logging module.
*       It boils down to two APIs that are automatically call before and 
*       after the main() function.
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
/*    PUBLIC TYPES                                                            */
/******************************************************************************/

typedef struct Log
{
    FILE *descriptor;
    char *name;
} LOG;

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

/* Macro to expose static functions to unit test runners */
#ifdef LOG_UNIT_TEST
    #define STATIC
#else
    #define STATIC static
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

/**
 * @brief   A getter of the LOG initial[3U] = {{stderr, "stderr"},...}};
 */
LOG* get();

/******************************************************************************/
/*    STATIC FUNCTION PROTOTYPES                                              */
/******************************************************************************/

STATIC LOG* set(LOG *newFile);

STATIC int32_t make(const char *dir);

STATIC char* make_name(const char *nameFormat);

STATIC LOG* open(char *filename);

STATIC LOG* init(uint32_t toFileFlag);

#ifdef __cplusplus
}
#endif

#endif /* FILE_H_ */
