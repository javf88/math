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
#include <ostream>
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

/******************************************************************************/
/*    PUBLIC MACROS                                                           */
/******************************************************************************/

/**
 * @example    LOG_ERROR(logger, "error var i =", i);
 */
#if LOG_LEVEL_ERROR <= LOG_CONFIG
    #define LOG_ERROR(LOGGER, ...) \
        LOGGER.log(Log::Level::ERROR, __FILE__, ":", __LINE__, " ", Log::MSG::ENDC, Log::MSG::GRAY, __VA_ARGS__, Log::MSG::ENDC)
#else
    #define LOG_ERROR(LOGGER, ...)
#endif

/**
 * @example    LOG_WARNING(logger, "warning var i =", i);
 */
#if LOG_LEVEL_WARNING <= LOG_CONFIG
    #define LOG_WARNING(LOGGER, ...) \
        LOGGER.log(Log::Level::WARNING, __FILE__, ":", __LINE__, " ", Log::MSG::ENDC, Log::MSG::GRAY, __VA_ARGS__, Log::MSG::ENDC)
#else
    #define LOG_WARNING(LOGGER, ...)
#endif

/**
 * @example    LOG_INFO(logger, "info var i =", i);
 */
#if LOG_LEVEL_INFO <= LOG_CONFIG
    #define LOG_INFO(LOGGER, ...) \
        LOGGER.log(Log::Level::INFO, __FILE__, ":", __LINE__, " ", Log::MSG::ENDC, Log::MSG::GRAY, __VA_ARGS__, Log::MSG::ENDC)
#else
    #define LOG_INFO(LOGGER, ...)
#endif

/**
 * @example    LOG_DEBUG(logger, "debug var i =", i);
 */
#if LOG_LEVEL_DEBUG <= LOG_CONFIG
    #define LOG_DEBUG(LOGGER, ...) \
        LOGGER.log(Log::Level::DEBUG, __FILE__, ":", __LINE__, " ", Log::MSG::ENDC, Log::MSG::GRAY, __VA_ARGS__, Log::MSG::ENDC)
#else
    #define LOG_DEBUG(LOGGER, ...)
#endif

/**
 * @example    LOG_TRACE(logger, "trace var i =", i);
 */
#if LOG_LEVEL_TRACE <= LOG_CONFIG
    #define LOG_TRACE(LOGGER, ...) \
        LOGGER.log(Log::Level::TRACE, __FILE__, ":", __LINE__, " ", Log::MSG::ENDC, Log::MSG::GRAY, __VA_ARGS__, Log::MSG::ENDC)
#else
    #define LOG_TRACE(LOGGER, ...)
#endif

/******************************************************************************/
/*    API                                                                     */
/******************************************************************************/

// Deriving from std::ostringstream because of the str() method
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

    // This is horrible, templates cannot be split into hpp/cpp files
    template<typename T, typename... Args>
    void log(const T& first, const Args&... args)
    {
        *this << first;
        log(args...);
    }


private:
    void log();

    // friend keyword is readable when implementation is split.
    // friend functions to ostream because it implements operator<<
    friend std::ostream& operator<<(std::ostream& os, const Log::Level level);

    friend std::ostream& operator<<(std::ostream& os, const Log::MSG fmt);
};

#endif /* LEVELS_H_ */
