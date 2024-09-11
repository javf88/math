#ifndef LOG_H_
#define LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/******************************************************************************/
/*    DEFINITIONS                                                             */
/******************************************************************************/

/* Define log levels */
#define LOG_LEVEL_ERROR   (0U)
#define LOG_LEVEL_WARNING (1U)
#define LOG_LEVEL_INFO    (2U)
#define LOG_LEVEL_DEBUG   (3U)
#define LOG_LEVEL_TRACE   (4U)

/* Define log boundaries */
#define LOG_LEVEL_FULL    LOG_LEVEL_TRACE

/* Set default log level to error (if not previously defined) */
#ifndef LOG_CONFIG
#define LOG_CONFIG        LOG_LEVEL_ERROR
#endif

/******************************************************************************/
/*    PRIVATE DATA                                                            */
/******************************************************************************/

static const char *levelFormat[] =
{
    "\x1b[31m[ ERROR ]\x1b[0m %s:%d", /* red */
    "\x1b[33m[WARNING]\x1b[0m %s:%d", /* yellow */
    "\x1b[32m[ INFO  ]\x1b[0m %s:%d", /* green */
    "\x1b[36m[ DEBUG ]\x1b[0m %s:%d", /* cyan */
    "\x1b[94m[ TRACE ]\x1b[0m %s:%d"  /* purple */
};

/******************************************************************************/
/*    IMPLEMENTATION                                                          */
/******************************************************************************/

char* _get_src(const int32_t level, const char *src, const uint32_t line)
{
    /* "65535" and "\0" are const strings of longest string for the cases */
    size_t size = strlen(src) + strlen("65535") + strlen("\0");
    const char *format = NULL;
    char *str = NULL;

    if (level <= LOG_LEVEL_FULL)
    {
        size += strlen(levelFormat[level]);
        str = malloc(sizeof(char) * size);

        sprintf(str, levelFormat[level], src, line);
    }

    return str;
}

char* _get_msg(const char *format, va_list args)
{
    /* refactor to compute actual length of ellipsis ... */
    char *str = malloc(sizeof(char) * 256U);

    vsprintf(str, format, args);

    return str;
}

/* log_print has not been unit-tested becasue it print to stderr and does not
 * return anything. Maybe when adding logging-into-file feature, this would
 * be tested. */
void log_print(unsigned int level, const char *src, const int line,
               const char *format, ...)
{
    char *srcStr = _get_src(level, src, line);
    char *msgStr = NULL;

    va_list args;
    va_start(args, format);
    msgStr = _get_msg(format, args);
    va_end(args);

    fprintf(stderr, "%s %s\n", srcStr, msgStr);

    free(srcStr);
    free(msgStr);
}

#ifdef __cplusplus
}
#endif

#endif /* LOG_H_ */
