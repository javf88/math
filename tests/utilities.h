/*******************************************************************************
*
* EXPERIMENTAL - Utilities submodule
*
*   SUMMARY
*       This submodule implements some unnecessary but yet useful information
*       at test function level, namnely:
*
*       a) file info
*          [ TEST ] ****  playground/math/tests/algebra/decomposition.c  ****
*
*       b) test info
*          [ TEST ] ******************   test_get_info   ******************
*
*******************************************************************************/

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include "levels.h"
#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <stdint.h>
#include <string.h>

/* Extending logging module for tests */
#include "log.h"

/******************************************************************************/
/*    PUBLIC TYPES                                                            */
/******************************************************************************/

typedef struct info
{
    const char   *margin;
    size_t       marginLength;
    const char   *bracket;
    size_t       bracketLength;
    const char   *str;
    size_t       strLength;
    const char   *padding;
    size_t       maxLineLength;
    const char   *color;
} INFO;

/******************************************************************************/
/*    PRIVATE DATA                                                            */
/******************************************************************************/

/* Important strings */
#define BOLD_BRIGHT_CYAN_START  "\x1b[1;96m"
#define CYAN_START  "\x1b[36m"
#define COLOR_END   "\x1b[0m"

#define MARGIN  "[ TEST ] /"
#define BRACKET "/"
#define PADDING "****************************************"

/* Important magic numbers */
#define SPACES_AROUND_NAME   (4U)
#define MAX_PRINTABLE_SIZE   (100U)

/******************************************************************************/
/*    API                                                                     */
/******************************************************************************/

void log_init(const char *submodule);

void log_info(const char *str);

/******************************************************************************/
/*    STATIC FUNCTION PROTOTYPES                                              */
/******************************************************************************/

STATIC INFO* get_info();

STATIC INFO* set_info(const char *str);

STATIC INFO* set_color(const char *color);

/******************************************************************************/
/*    IMPLEMENTATION                                                          */
/******************************************************************************/

void log_init(const char *submodule)
{
    INFO *info = set_color(BOLD_BRIGHT_CYAN_START);
    log_info(submodule);
    /* After initialization we print in a CYAN */
    info = set_color(CYAN_START);
}

void log_info(const char *str)
{
    INFO *info = set_info(str);
    /* MAX_STR_LEN from levels.h */
    char buffer[MAX_STR_LEN];
    size_t pos = 0U;
    int32_t padding = info->maxLineLength;

    padding -= info->marginLength + info->bracketLength + info->strLength;
    padding = padding / 2U;

    pos = sprintf(&buffer[pos], "%s", info->color);
    /* Printing "[ TEST ] /.**..." */
    pos = sprintf(&buffer[pos], "%s%.*s  ", info->margin, padding, info->padding);
    pos += sprintf(&buffer[pos], "%s", info->str);
    /* Printing "..**./" */
    pos += sprintf(&buffer[pos], "  %.*s/", padding, info->padding);
    pos = sprintf(&buffer[pos], "%s", COLOR_END);

    log_tee(buffer);
}

STATIC INFO* get_info()
{
    /* As a singleton, it exists just once, one per TU  */
    static INFO info = {MARGIN, strlen(MARGIN),
                        BRACKET, strlen(BRACKET),
                        NULL, 0U,
                        PADDING, MAX_PRINTABLE_SIZE};

    return &info;
}

STATIC INFO* set_info(const char *str)
{
    INFO *info = get_info();
    info->str = str;
    info->strLength = strlen(str) + SPACES_AROUND_NAME;

    return info;
}

STATIC INFO* set_color(const char *color)
{
    INFO *info = get_info();
    info->color = color;

    return info;
}

#ifdef __cplusplus
}
#endif

#endif /* UTILITIES_H_ */
