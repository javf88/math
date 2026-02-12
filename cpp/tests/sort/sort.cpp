/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <gtest/gtest.h>
/* TARGET LIBRARY */
#include "sort.hpp"

/******************************************************************************/
/*    TEST CASES                                                              */
/******************************************************************************/

TEST(file, Constructor)
{
    File file(LOG_TO_FILE);
}

/*
#if LOG_LEVEL_ERROR <= LOG_CONFIG
TEST_F(Verbose, error)
{
    Log error;

    LOG_ERROR(error, "error");
    expected.log(Log::Level::ERROR, __FILE__, ":", __LINE__ - 1, " ", Log::MSG::ENDC, Log::MSG::GRAY, "error", Log::MSG::ENDC);
    ASSERT_EQ(this->expected.str(), error.str());
}
#endif

#if LOG_LEVEL_WARNING <= LOG_CONFIG
TEST_F(Verbose, warning)
{
    Log warning;

    LOG_WARNING(warning, "warning");
    expected.log(Log::Level::WARNING, __FILE__, ":", __LINE__ - 1, " ", Log::MSG::ENDC, Log::MSG::GRAY, "warning", Log::MSG::ENDC);
    ASSERT_EQ(this->expected.str(), warning.str());
}
#endif

#if LOG_LEVEL_INFO <= LOG_CONFIG
TEST_F(Verbose, info)
{
    Log info;

    LOG_INFO(info, "info");
    expected.log(Log::Level::INFO, __FILE__, ":", __LINE__ - 1, " ", Log::MSG::ENDC, Log::MSG::GRAY, "info", Log::MSG::ENDC);
    ASSERT_EQ(this->expected.str(), info.str());
}
#endif

#if LOG_LEVEL_DEBUG <= LOG_CONFIG
TEST_F(Verbose, debug)
{
    Log debug;

    LOG_DEBUG(debug, "debug");
    expected.log(Log::Level::DEBUG, __FILE__, ":", __LINE__ - 1, " ", Log::MSG::ENDC, Log::MSG::GRAY, "debug", Log::MSG::ENDC);
    ASSERT_EQ(this->expected.str(), debug.str());
}
#endif

#if LOG_LEVEL_TRACE <= LOG_CONFIG
TEST_F(Verbose, trace)
{
    Log trace;

    LOG_TRACE(trace, "trace");
    expected.log(Log::Level::TRACE, __FILE__, ":", __LINE__ - 1, " ", Log::MSG::ENDC, Log::MSG::GRAY, "trace", Log::MSG::ENDC);
    ASSERT_EQ(this->expected.str(), trace.str());
}
#endif
*/
