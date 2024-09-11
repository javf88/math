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
/*    PUBLIC FUNCTION-LIKE MACRO                                             */
/******************************************************************************/

/**
 * @brief      Prints Error-level message
 *
 * @param      ...   Message format + optional arguments
 *
 * @example    LOG_ERROR("socket: %s", strerror(errno));
 */
#if LOG_LEVEL_ERROR <= LOG_CONFIG
    #define LOG_ERROR(...) \
        log_print(LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#else
    #define LOG_ERROR(...)
#endif

/**
 * @brief      Prints Warning-level message
 *
 * @param      ...   Message format + optional arguments
 *
 * @example    LOG_WARNING("Unknown sensor ID %u", sensor_id);
 */
#if LOG_LEVEL_WARNING <= LOG_CONFIG
    #define LOG_WARNING(...) \
    log_print(LOG_LEVEL_WARNING, __FILE__, __LINE__, __VA_ARGS__)
#else
    #define LOG_WARNING(...)
#endif

/**
 * @brief      Prints Info-level message
 *
 * @param      ...   Message format + optional arguments
 *
 * @example    LOG_INFO("Successful initialization");
 */
#if LOG_LEVEL_INFO <= LOG_CONFIG
    #define LOG_INFO(...) \
    log_print(LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__)
#else
    #define LOG_INFO(...)
#endif

/**
 * @brief      Prints Debug-level message
 *
 * @param      ...   Message format + optional arguments
 *
 * @example    LOG_DEBUG("Function 'Timer3_ISR' reached");
 */
#if LOG_LEVEL_DEBUG <= LOG_CONFIG
    #define LOG_DEBUG(...) \
    log_print(LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#else
    #define LOG_DEBUG(...)
#endif

/**
 * @brief      Prints Trace-level message
 *
 * @param      ...   Message format + optional arguments
 *
 * @example    LOG_TRACE("CAN Message ID %d received", m_id);
 */
#if LOG_LEVEL_TRACE <= LOG_CONFIG
    #define LOG_TRACE(...) \
    log_print(LOG_LEVEL_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#else
    #define LOG_TRACE(...)
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
