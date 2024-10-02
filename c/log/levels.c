/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include "levels.h"

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

static FILE *toFile = NULL;

/* Buffers' magic numbers */
#define MAX_STR_LEN   (256U)

/******************************************************************************/
/*    STATIC FUNCTION PROTOTYPES                                              */
/******************************************************************************/

static char* get_src(const uint32_t level, const char *src, const uint32_t line);

static char* get_msg(const char *format, const va_list args);

static void tee_printf(const char *str, FILE *toFile);

/******************************************************************************/
/*    IMPLEMENTATION                                                          */
/******************************************************************************/

void log_print(const uint32_t level, const char *src, const uint32_t line,
               const char *format, ...)
{
    char  *srcStr = get_src(level, src, line);
    char *msgStr = NULL;
    char buffer[MAX_STR_LEN];

    va_list args;
    va_start(args, format);
    msgStr = get_msg(format, args);
    va_end(args);

    sprintf(buffer, "%s "BOLD_GRAY("%s")"\n", srcStr, msgStr);
    tee_printf(buffer, toFile);

    free(srcStr);
    free(msgStr);
}

void log_matrix(const uint32_t level, const char *src, const uint32_t line,
                const char *name, const float *val, const uint32_t rows,
                const uint32_t cols)
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

        tee_printf(buffer, toFile);
        /* Clearing word with "    "  */
        sprintf(word, "%8s", "");
    }
}

static char* get_src(const uint32_t level, const char *src, const uint32_t line)
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

static char* get_msg(const char *format, const va_list args)
{
    /* refactor to compute actual length of ellipsis ... */
    char *str = malloc(sizeof(char) * MAX_STR_LEN);

    vsprintf(str, format, args);

    return str;
}

static void tee_printf(const char *str, FILE *toFile)
{
    char buffer[MAX_STR_LEN];

    fprintf(stderr, "%s", str);
    if (toFile != NULL)
    {
        fprintf(toFile, "%s", str);
    }
}
