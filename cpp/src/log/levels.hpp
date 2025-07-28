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
#include <sstream>

/******************************************************************************/
/*    PUBLIC MACROS                                                           */
/******************************************************************************/

/**
 * @example    LOG_ERROR(logger, "error var i =", i);
 */
#if LOG_LEVEL_ERROR <= LOG_CONFIG
    #define LOG_ERROR(LOGGER, ...) \
        LOGGER.log(Log::Level::ERROR, __FILE__, ":", __LINE__, __VA_ARGS__)
#else
    #define LOG_ERROR(LOGGER, ...)
#endif

/**
 * @example    LOG_WARNING(logger, "warning var i =", i);
 */
#if LOG_LEVEL_WARNING <= LOG_CONFIG
    #define LOG_WARNING(LOGGER, ...) \
        LOGGER.log(Log::Level::WARNING, __FILE__, ":", __LINE__, __VA_ARGS__)
#else
    #define LOG_WARNING(LOGGER, ...)
#endif

/**
 * @example    LOG_INFO(logger, "info var i =", i);
 */
#if LOG_LEVEL_INFO <= LOG_CONFIG
    #define LOG_INFO(LOGGER, ...) \
        LOGGER.log(Log::Level::INFO, __FILE__, ":", __LINE__, __VA_ARGS__)
#else
    #define LOG_INFO(LOGGER, ...)
#endif

/**
 * @example    LOG_DEBUG(logger, "debug var i =", i);
 */
#if LOG_LEVEL_DEBUG <= LOG_CONFIG
    #define LOG_DEBUG(LOGGER, ...) \
        LOGGER.log(Log::Level::DEBUG, __FILE__, ":", __LINE__, __VA_ARGS__)
#else
    #define LOG_DEBUG(LOGGER, ...)
#endif

/**
 * @example    LOG_TRACE(logger, "trace var i =", i);
 */
#if LOG_LEVEL_TRACE <= LOG_CONFIG
    #define LOG_TRACE(LOGGER, ...) \
        LOGGER.log(Log::Level::TRACE, __FILE__, ":", __LINE__, __VA_ARGS__)
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
        BEGIN = 0U,
        ENDL
    };

    void log()
    {
        //Disable coloring
        *this << Log::MSG::ENDL;
    }

    template<typename T, typename... Args>
    void log(const T& first, const Args&... args)
    {
        *this << first;
        log(args...);
    }

    // Horrible friend function, it forces to write the look-up table
    // inside the function.
    friend std::ostream& operator<<(std::ostream& os, const Log::Level level)
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

    friend std::ostream& operator<<(std::ostream& os, const Log::MSG fmt)
    {
        // Look-up table
        const char *color[2U] =
        {
            "\x1b[0m \x1b[1;90m", //Bold gray
            "\x1b[0m\n"           //Disable coloring
        };

        os << color[fmt];
        return os;
    }

    // Log log;
    // log << Log::ERROR << __FILE__ << __LINE__;
    // LOG_ERROR(__FILE__, __LINE__);
};

/******************************************************************************/
/*    IMPLEMENTATION                                                          */
/******************************************************************************/

#endif /* LEVELS_H_ */
