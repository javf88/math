/*******************************************************************************
*
* LOGGING SYSTEM - levels submodule
*
*   SUMMARY
*       This submodule implements two APIs for logging-purpose.
*
*       a) log_print() is a function that logs text within five levels, in
*          different colors.
*
*       b) log_matrix() is a specialization for logging the MATRIX typedef.
*
*       c) a set of macros conseal the main APIs to ease the use of this
*          submodule.
*
*******************************************************************************/

#ifndef LEVELS_H_
#define LEVELS_H_

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

#include "file.h"

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
#define LOG_LEVEL_FULL      LOG_LEVEL_TRACE

/* Set default log level to error (if not previously defined) */
#ifndef LOG_CONFIG
    #define LOG_CONFIG      LOG_LEVEL_ERROR
#endif

/* Macro to expose static functions to unit test runners */
#ifdef LOG_UNIT_TEST
    #define STATIC
#else
    #define STATIC static
#endif

/* Buffers' magic numbers */
#define MAX_STR_LEN   (256U)

/* Colors for matrix*/
#define BOLD_GRAY(fmt) "\x1b[1;90m"fmt"\x1b[0m"
#define WHITE(fmt)     "\x1b[37m"fmt"\x1b[0m"

/* Colors for log messages */
#define RED(fmt)       "\x1b[31m"fmt"\x1b[0m"
#define YELLOW(fmt)    "\x1b[33m"fmt"\x1b[0m"
#define GREEN(fmt)     "\x1b[32m"fmt"\x1b[0m"
#define CYAN(fmt)      "\x1b[36m"fmt"\x1b[0m"
#define PURPLE(fmt)    "\x1b[94m"fmt"\x1b[0m"

/******************************************************************************/
/*    PUBLIC MACROS                                                           */
/******************************************************************************/

/**
 * @example    LOG_ERROR("socket: %s", strerror(errno));
 */
#if LOG_LEVEL_ERROR <= LOG_CONFIG
    #define LOG_ERROR(...) \
        log_print(LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#else
    #define LOG_ERROR(...)
#endif

/**
 * @example    LOG_WARNING("Unknown sensor ID %u", sensor_id);
 */
#if LOG_LEVEL_WARNING <= LOG_CONFIG
    #define LOG_WARNING(...) \
        log_print(LOG_LEVEL_WARNING, __FILE__, __LINE__, __VA_ARGS__)
#else
    #define LOG_WARNING(...)
#endif

/**
 * @example    LOG_INFO("Successful initialization");
 */
#if LOG_LEVEL_INFO <= LOG_CONFIG
    #define LOG_INFO(...) \
        log_print(LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__)
#else
    #define LOG_INFO(...)
#endif

/**
 * @example    LOG_DEBUG("Function 'Timer3_ISR' reached");
 */
#if LOG_LEVEL_DEBUG <= LOG_CONFIG
    #define LOG_DEBUG(...) \
        log_print(LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#else
    #define LOG_DEBUG(...)
#endif

/**
 * @example    LOG_TRACE("CAN Message ID %d received", m_id);
 */
#if LOG_LEVEL_TRACE <= LOG_CONFIG
    #define LOG_TRACE(...) \
        log_print(LOG_LEVEL_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#else
    #define LOG_TRACE(...)
#endif

/**
 * @example    LOG_WARNING_MATRIX(A);
 */
#if LOG_LEVEL_WARNING <= LOG_CONFIG
    #define LOG_WARNING_MATRIX(A) \
        log_matrix(LOG_LEVEL_WARNING, __FILE__, __LINE__, #A, A->val, A->rows, A->cols)
#else
    #define LOG_WARNING_MATRIX(A)
#endif

/**
 * @example    LOG_INFO_MATRIX(A);
 */
#if LOG_LEVEL_INFO <= LOG_CONFIG
    #define LOG_INFO_MATRIX(A) \
        log_matrix(LOG_LEVEL_INFO, __FILE__, __LINE__, #A, A->val, A->rows, A->cols)
#else
    #define LOG_INFO_MATRIX(A)
#endif

/**
 * @example    LOG_DEBUG_MATRIX(A);
 */
#if LOG_LEVEL_DEBUG <= LOG_CONFIG
    #define LOG_DEBUG_MATRIX(A) \
        log_matrix(LOG_LEVEL_DEBUG, __FILE__, __LINE__, #A, A->val, A->rows, A->cols)
#else
    #define LOG_DEBUG_MATRIX(A)
#endif

/**
 * @example    LOG_TRACE_MATRIX(A);
 */
#if LOG_LEVEL_TRACE <= LOG_CONFIG
    #define LOG_TRACE_MATRIX(A) \
        log_matrix(LOG_LEVEL_TRACE, __FILE__, __LINE__, #A, A->val, A->rows, A->cols)
#else
    #define LOG_TRACE_MATRIX(A)
#endif

/******************************************************************************/
/*    API                                                                     */
/******************************************************************************/

/**
 * @brief   Function that prints to streams
 *
 * @examples log_print(LOG_LEVEL_ERROR, __FILE__, __LINE__, "my test %d", 50);
 */
void log_print(const uint32_t level, const char *src,
               const uint32_t line, const char *format, ...);

/**
 * @brief   Function that prints a matrix to streams
 *
 * @examples log_matrix(LOG_LEVEL_INFO, __FILE__, __LINE__, "A", A->val, A->rows, A->cols);
 */
void log_matrix(const uint32_t level, const char *src, const uint32_t line,
                const char *name, const float *val, const uint32_t rows,
                const uint32_t cols);

/**
 * @brief   Function that prints to stderr and FILE* streams
 *
 * @examples log_tee(__FILE__)
 */
void log_tee(const char *str);

/******************************************************************************/
/*    STATIC FUNCTION PROTOTYPES                                              */
/******************************************************************************/

STATIC char* get_src(const uint32_t level, const char *src, const uint32_t line);

STATIC char* get_msg(const char *format, const va_list args);

#ifdef __cplusplus
}
#endif

#endif /* LEVELS_H_ */
