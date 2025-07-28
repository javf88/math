/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include "levels.h"

/******************************************************************************/
/*    PRIVATE DATA                                                            */
/******************************************************************************/

static const char *levelFormat[] =
{
    RED("[ ERROR ] %s:%d"),
    YELLOW("[WARNING] %s:%d"),
    GREEN("[ INFO  ] %s:%d"),
    CYAN("[ DEBUG ] %s:%d"),
    PURPLE("[ TRACE ] %s:%d")
};

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
    log_tee(buffer);

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

        log_tee(buffer);
        /* Clearing word with "    "  */
        sprintf(word, "%8s", "");
    }
}

void log_tee(const char *str)
{
    LOG *file = get();
    char buffer[MAX_STR_LEN];

    for (LOG *itr = file; itr->name != NULL; itr++)
    {
        fprintf(itr->descriptor, "%s", str);
    }
}

STATIC char* get_src(const uint32_t level, const char *src, const uint32_t line)
{
    /* "65535" and "\0" are const strings of longest string for the cases */
    size_t size = strlen(src) + strlen("65535") + strlen("\0");
    const char *format = NULL;
    char *str = NULL;

    if (level <= LOG_LEVEL_FULL)
    {
        size += strlen(levelFormat[level]);
        str = malloc(sizeof(char) * size);
        // log << RED(__FILE__,__LINE__);
        // with caching posibility
        // log << Level::ERROR << __FILE__ << __LINE__ << Message << "bla bla bla" << std::endl;
        // LOG_ERROR("bla bla bla");
        sprintf(str, levelFormat[level], src, line);
    }

    return str;
}

STATIC char* get_msg(const char *format, const va_list args)
{
    /* refactor to compute actual length of ellipsis ... */
    char *str = malloc(sizeof(char) * MAX_STR_LEN);

    vsprintf(str, format, args);

    return str;
}
