/*******************************************************************************
*
* LOGGING SYSTEM
*
*   SUMMARY
*       This single-header module implements a simple logging system.
*
*******************************************************************************/

#ifndef LOG_H_
#define LOG_H_

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <sys/stat.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>

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

/**
 * @brief      Prints a matrix in info level
 *
 * @param      A   Matrix variable
 */
#if LOG_LEVEL_INFO <= LOG_CONFIG
#define LOG_INFO_MATRIX(A) \
    log_matrix(LOG_LEVEL_INFO, __FILE__, __LINE__, #A, A->val, A->rows, A->cols)
#else
#define LOG_INFO_MATRIX(A)
#endif

/******************************************************************************/
/*    PRIVATE DATA                                                            */
/******************************************************************************/

/* Colors for matrix*/
#define BOLD_GRAY(fmt) "\x1b[1;90m"fmt"\x1b[0m"
#define WHITE(fmt)     "\x1b[37m"fmt"\x1b[0m"

/* Colors for log messages */
#define RED(fmt)       "\x1b[31m"fmt"\x1b[0m"
#define YELLOW(fmt)    "\x1b[33m"fmt"\x1b[0m"
#define GREEN(fmt)     "\x1b[32m"fmt"\x1b[0m"
#define CYAN(fmt)      "\x1b[36m"fmt"\x1b[0m"
#define PURPLE(fmt)    "\x1b[94m"fmt"\x1b[0m"

static const char *levelFormat[] =
{
    RED("[ ERROR ] %s:%d"),
    YELLOW("[WARNING] %s:%d"),
    GREEN("[ INFO  ] %s:%d"),
    CYAN("[ DEBUG ] %s:%d"),
    PURPLE("[ TRACE ] %s:%d")
};

/* Buffers' magic numbers */
#define MAX_STR_LEN   (256U)
#define MAX_WORD_LEN  (25U)
#define MAX_NAME_LEN  (50U)

/* It seems that the LOG_TO_FILE-code should of in it own c-file.
 * Code management would be easier. */
#ifdef LOG_TO_FILE
typedef struct Log
{
    FILE *descriptor;
    char *name;
} LOG;

/* file handler */
static LOG *file = NULL;

#endif


/******************************************************************************/
/*    IMPLEMENTATION                                                          */
/******************************************************************************/

char* _get_src(const uint32_t level, const char *src, const uint32_t line)
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

char* _get_msg(const char *format, const va_list args)
{
    /* refactor to compute actual length of ellipsis ... */
    char *str = malloc(sizeof(char) * MAX_STR_LEN);

    vsprintf(str, format, args);

    return str;
}

/* buffer needs to be reafctored such that we can print to file and stderr */
void tee_printf(const char *str)
{
    char buffer[MAX_STR_LEN];

    fprintf(stderr, "%s", str);
#ifdef LOG_TO_FILE
    if (file != NULL)
    {
        /*
        if (file->descriptor != NULL)
        {
            fprintf(file->descriptor, "%s", str);
        }
        */
    } 
#endif
}

void log_print(const uint32_t level, const char *src, const uint32_t line,
               const char *format, ...)
{
    char  *srcStr = _get_src(level, src, line);
    char *msgStr = NULL;
    char buffer[MAX_STR_LEN];

    va_list args;
    va_start(args, format);
    msgStr = _get_msg(format, args);
    va_end(args);

    sprintf(buffer, "%s "BOLD_GRAY("%s")"\n", srcStr, msgStr);
    tee_printf(buffer);

    free(srcStr);
    free(msgStr);
}

/* The function prints a matrix in a tabular-fashion and align to eaze
 * the debugging of matrices
 *
 * Ex.  A = [ 1.000, 1.000, 1.000, 1.000, 1.000]
 *          [ 1.000, 1.000, 1.000, 1.000, 1.000]
 *          [ 1.000, 1.000, 1.000, 1.000, 1.000]
 * */
void log_matrix(const uint32_t level, const char *src, const uint32_t line,
                const char *name, const float *val, const uint32_t rows, const uint32_t cols)
{
    /* Building " A = " */
    char word[MAX_STR_LEN / 10U];
    char buffer[MAX_STR_LEN];

    log_print(level, src, line, BOLD_GRAY("(MATRIX)%s in [%ux%u]"), name, rows, cols);
    sprintf(word, "%5s = ", name);

    for (uint32_t i = 0U, k = 0U; i < rows; i++, k = 0U)
    {
        /* Building "    " */
        k += sprintf(&buffer[k], BOLD_GRAY("%8s["), word);

        for (uint32_t j = 0U; j < (cols - 1U); j++)
        {
            /* Building " d.ddddddd, " in a 11-char column */
            uint32_t pos = cols * i + j;
            sprintf(word, "%.7f", val[pos]);
            k += sprintf(&buffer[k], WHITE("%11s, "), word);
        }

        /* Building last column " d.ddddddd]" */
        sprintf(word, "%.7f", val[cols * i + cols - 1U]);
        k += sprintf(&buffer[k], WHITE("%11s")BOLD_GRAY("]")"\n", word);

        tee_printf(buffer);
        /* Clearing word with "    "  */
        sprintf(word, "%8s", "");
    }
}

#ifdef LOG_TO_FILE
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

int32_t make(const char *dir)
{
    /* Creating rel dir */
    mkdir(dir, 0777);
    switch (errno)
    {
        case 0:
            LOG_INFO("Directory %s was created. [CODE %d]", dir, errno);
            break;
        case EEXIST:
            LOG_INFO("Directory %s exists already. [CODE %d]", dir, errno);
            break;
        default:
            /* If dir is not created, opening file routine cleans up */
            LOG_WARNING("Directory %s was not created! [CODE %d]", dir, errno);
            break;
    }

    return errno;
}

int32_t name(LOG *file)
{
    int32_t ret ;

    if (LOG_TO_FILE == 1U)
    {
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
    }

    return ret;
}

LOG* init(LOG *files)
{
    const LOG vec[2U] = {{stderr, "stderr"}, {NULL, NULL}};

    if (files != NULL)
    {
        fprintf(stderr, "Logging module was not initialized.\n");
        return files;
    }

    /* LOG_TO_FILE is either 0U or 1U */
    files = (LOG*)malloc(sizeof(LOG) * 2U + LOG_TO_FILE);
    memcpy(&files[LOG_TO_FILE], vec, sizeof(LOG) * 2U);

    return files;
}

void constructor(LOG *file)
{
    int32_t exists;
    file = init(file);
    exists = name(file);
    if (exists > -1)
    {
        open(file);
    }
}

#endif

#ifdef LOG_TO_FILE
LOG* _file_close(LOG *file)
{
    LOG_TRACE("%s is run.", __func__);
    if (file != NULL)
    {
        LOG_INFO("Closing file %s", file->name);
        fflush(file->descriptor);
        fclose(file->descriptor);
        free(file->name);
        free(file);

        file->descriptor = NULL;
        file->name = NULL;
        file = NULL;
        LOG_INFO("file->descriptor = NULL");
    }

    return NULL;
}
#endif

#ifdef __cplusplus
}
#endif

#endif /* LOG_H_ */
