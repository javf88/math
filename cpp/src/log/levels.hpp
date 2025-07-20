/*******************************************************************************
*
*   SUMMARY
*
*******************************************************************************/

#ifndef LEVELS_H_
#define LEVELS_H_

/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

/******************************************************************************/
/*    API                                                                     */
/******************************************************************************/

struct Log
{
    enum Level: uint32_t
    {
        ERROR = 0U,
        WARNING,
        INFO,
        DEBUG,
        TRACE,
        FULL
    };

    const char *format[5U] =
    {
        "[ ERROR ] %s:%d",
        "[WARNING] %s:%d",
        "[ INFO  ] %s:%d",
        "[ DEBUG ] %s:%d",
        "[ TRACE ] %s:%d"

    };

    // To do:
    // To use itoc for strlen("65535")
    // To replace char* with string
    // To replace strlen and malloc wirh string's capabilities
    // To replace sprintf with osstream
    char* getSrc(const uint32_t level, const char *src, const uint32_t line)
    {
        /* "65535" and "\0" are const strings of longest string for the cases */
        size_t size = strlen(src) + strlen("65535") + strlen("\0");
        const char *format = NULL;
        char *str = NULL;

        if (level < Level::FULL)
        {
            size += std::strlen(this->format[level]);
            str = (char*)std::malloc(sizeof(char) * size);

            std::sprintf(str, this->format[level], src, line);
        }

        return str;
    }
};

/******************************************************************************/
/*    IMPLEMENTATION                                                          */
/******************************************************************************/

#endif /* LEVELS_H_ */
