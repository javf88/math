/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <gtest/gtest.h>
/* TARGET LIBRARY */
#include "levels.hpp"

/******************************************************************************/
/*    TEST CASES                                                              */
/******************************************************************************/

TEST(Log, error)
{
    Log expected;
    Log error;

    LOG_ERROR(error, "error");
    expected.log(Log::Level::ERROR, __FILE__, ":", __LINE__ - 1, "error");
    ASSERT_EQ(expected.str(), error.str());
}

TEST(Log, warning)
{
    Log expected;
    Log warning;

    LOG_WARNING(warning, "warning");
    expected.log(Log::Level::WARNING, __FILE__, ":", __LINE__ - 1, "warning");
    ASSERT_EQ(expected.str(), warning.str());
}

TEST(Log, info)
{
    Log expected;
    Log info;

    LOG_INFO(info, "info");
    expected.log(Log::Level::INFO, __FILE__, ":", __LINE__ - 1, "info");
    ASSERT_EQ(expected.str(), info.str());
}

TEST(Log, debug)
{
    Log expected;
    Log debug;

    LOG_DEBUG(debug, "debug");
    expected.log(Log::Level::DEBUG, __FILE__, ":", __LINE__ - 1, "debug");
    ASSERT_EQ(expected.str(), debug.str());
}

TEST(Log, trace)
{
    Log expected;
    Log trace;

    LOG_TRACE(trace, "trace");
    expected.log(Log::Level::TRACE, __FILE__, ":", __LINE__ - 1, "trace");
    ASSERT_EQ(expected.str(), trace.str());
}
