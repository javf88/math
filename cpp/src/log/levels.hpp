/*******************************************************************************
*
* LOGGING SYSTEM - levels submodule
*
*   SUMMARY
*       This submodule implements APIs for logging-purpose.
*
*       a) struct Log is an extension of std::ostringstream that provides
*          basic logging capabilities.
*
*       c) a set of macros conseal the main APIs to ease the use of this
*          submodule.
*
*******************************************************************************/

#ifndef LEVELS_H_
#define LEVELS_H_

/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <cstdint>
#include <sstream>
#include <iostream>

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

/******************************************************************************/
/*    PUBLIC MACROS                                                           */
/******************************************************************************/

/**
 * @example    LOG_ERROR(logger, "error var i =", i);
 */
#if LOG_LEVEL_ERROR <= LOG_CONFIG
    #define LOG_ERROR(LOGGER, ...) \
        LOGGER.log(Log::Level::ERROR, __FILE__, ":", __LINE__, " ", Log::MSG::ENDC, Log::MSG::GRAY, __VA_ARGS__)
#else
    #define LOG_ERROR(LOGGER, ...)
#endif

/**
 * @example    LOG_WARNING(logger, "warning var i =", i);
 */
#if LOG_LEVEL_WARNING <= LOG_CONFIG
    #define LOG_WARNING(LOGGER, ...) \
        LOGGER.log(Log::Level::WARNING, __FILE__, ":", __LINE__, " ", Log::MSG::ENDC, Log::MSG::GRAY, __VA_ARGS__)
#else
    #define LOG_WARNING(LOGGER, ...)
#endif

/**
 * @example    LOG_INFO(logger, "info var i =", i);
 */
#if LOG_LEVEL_INFO <= LOG_CONFIG
    #define LOG_INFO(LOGGER, ...) \
        LOGGER.log(Log::Level::INFO, __FILE__, ":", __LINE__, " ", Log::MSG::ENDC, Log::MSG::GRAY, __VA_ARGS__)
#else
    #define LOG_INFO(LOGGER, ...)
#endif

/**
 * @example    LOG_DEBUG(logger, "debug var i =", i);
 */
#if LOG_LEVEL_DEBUG <= LOG_CONFIG
    #define LOG_DEBUG(LOGGER, ...) \
        LOGGER.log(Log::Level::DEBUG, __FILE__, ":", __LINE__, " ", Log::MSG::ENDC, Log::MSG::GRAY, __VA_ARGS__)
#else
    #define LOG_DEBUG(LOGGER, ...)
#endif

/**
 * @example    LOG_TRACE(logger, "trace var i =", i);
 */
#if LOG_LEVEL_TRACE <= LOG_CONFIG
    #define LOG_TRACE(LOGGER, ...) \
        LOGGER.log(Log::Level::TRACE, __FILE__, ":", __LINE__, " ", Log::MSG::ENDC, Log::MSG::GRAY, __VA_ARGS__)
#else
    #define LOG_TRACE(LOGGER, ...)
#endif

/******************************************************************************/
/*    API                                                                     */
/******************************************************************************/

struct Log: public std::ostringstream
{
    // enums as key for the look-up tables.
    enum Level: uint32_t
    {
        ERROR = 0U,
        WARNING,
        INFO,
        DEBUG,
        TRACE,
        FULL
    };

    enum MSG: uint32_t
    {
        GRAY = 0U,
        WHITE,
        ENDC,
        ENDL
    };

    void log();

    template<typename T, typename... Args>
    void log(const T& first, const Args&... args);

    // friend keyword is readable when implementation is split.
    friend std::ostream& operator<<(std::ostream& os, const Log::Level level);

    friend std::ostream& operator<<(std::ostream& os, const Log::MSG fmt);
};

/******************************************************************************/
/*    IMPLEMENTATION                                                          */
/******************************************************************************/

void Log::log()
{
    //Disable coloring
    *this << Log::MSG::ENDC << Log::MSG::ENDL;

    // To change later for a std* or even a file
    std::cout << this->str();
}

template<typename T, typename... Args>
void Log::log(const T& first, const Args&... args)
{
    *this << first;
    log(args...);
}

// no need of friend keyword. When splitting implementations and declarations
// it doesnt look that messy.
std::ostream& operator<<(std::ostream& os, const Log::Level level)
{
    // Look-up table
    const char *label[6U] =
    {
        "\x1b[31m[ ERROR ] ",
        "\x1b[33m[WARNING] ",
        "\x1b[32m[ INFO  ] ",
        "\x1b[36m[ DEBUG ] ",
        "\x1b[94m[ TRACE ] "
    };

    os << label[level];
    return os;
}

std::ostream& operator<<(std::ostream& os, const Log::MSG fmt)
{
    // Look-up table
    const char *color[4U] =
    {
        "\x1b[1;90m", //Bold gray
        "\x1b[37m",   // White
        "\x1b[0m",            //Disable coloring
        "\n"
    };

    os << color[fmt];
    return os;
}

#endif /* LEVELS_H_ */
