/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <gtest/gtest.h>
/* TARGET LIBRARY */
#include "levels.hpp"

/******************************************************************************/
/*    FIXTURES                                                                */
/******************************************************************************/

class Verbose: public testing::Test
{
protected:
    Log expected;

    // The Dtor triggers the flush to the stream. This is only for displaying
    // purpose. It was moved from the Log class in levels.hpp.
    ~Verbose()
    {
        std::cout << expected.str();
    }
};

/******************************************************************************/
/*    TEST CASES                                                              */
/******************************************************************************/

TEST_F(Verbose, error)
{
    Log error;

    LOG_ERROR(error, "error");
    expected.log(Log::Level::ERROR, __FILE__, ":", __LINE__ - 1, " ", Log::MSG::ENDC, Log::MSG::GRAY, "error", Log::MSG::ENDC, Log::MSG::ENDL);
    ASSERT_EQ(this->expected.str(), error.str());
}

TEST_F(Verbose, warning)
{
    Log warning;

    LOG_WARNING(warning, "warning");
    expected.log(Log::Level::WARNING, __FILE__, ":", __LINE__ - 1, " ", Log::MSG::ENDC, Log::MSG::GRAY, "warning", Log::MSG::ENDC, Log::MSG::ENDL);
    ASSERT_EQ(this->expected.str(), warning.str());
}

TEST_F(Verbose, info)
{
    Log info;

    LOG_INFO(info, "info");
    expected.log(Log::Level::INFO, __FILE__, ":", __LINE__ - 1, " ", Log::MSG::ENDC, Log::MSG::GRAY, "info", Log::MSG::ENDC, Log::MSG::ENDL);
    ASSERT_EQ(this->expected.str(), info.str());
}

TEST_F(Verbose, debug)
{
    Log debug;

    LOG_DEBUG(debug, "debug");
    expected.log(Log::Level::DEBUG, __FILE__, ":", __LINE__ - 1, " ", Log::MSG::ENDC, Log::MSG::GRAY, "debug", Log::MSG::ENDC, Log::MSG::ENDL);
    ASSERT_EQ(this->expected.str(), debug.str());
}

TEST_F(Verbose, trace)
{
    Log trace;

    LOG_TRACE(trace, "trace");
    expected.log(Log::Level::TRACE, __FILE__, ":", __LINE__ - 1, " ", Log::MSG::ENDC, Log::MSG::GRAY, "trace", Log::MSG::ENDC, Log::MSG::ENDL);
    ASSERT_EQ(this->expected.str(), trace.str());
}
