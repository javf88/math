/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <ostream>
#include <gtest/gtest.h>
/* TARGET LIBRARY */
#include "levels.hpp"

/******************************************************************************/
/*    TEST CASES                                                              */
/******************************************************************************/

TEST(Log, getSrc)
{
    std::ostringstream expected;
    Log error;
    uint32_t line = __LINE__;
    char *pMsg = nullptr;

    pMsg = error.getSrc(Log::Level::ERROR, __FILE__, line);
    expected << "[ ERROR ] " << __FILE__ << ':' <<  line;
    ASSERT_EQ(expected.str(), pMsg);
    expected.str("");
    expected.clear();

    pMsg = error.getSrc(Log::Level::WARNING, __FILE__, line);
    expected << "[WARNING] " << __FILE__ << ':' <<  line;
    ASSERT_EQ(expected.str(), pMsg);
    expected.str("");
    expected.clear();

    pMsg = error.getSrc(Log::Level::INFO, __FILE__, line);
    expected << "[ INFO  ] " << __FILE__ << ':' <<  line;
    ASSERT_EQ(expected.str(), pMsg);
    expected.str("");
    expected.clear();

    pMsg = error.getSrc(Log::Level::DEBUG, __FILE__, line);
    expected << "[ DEBUG ] " << __FILE__ << ':' <<  line;
    ASSERT_EQ(expected.str(), pMsg);
    expected.str("");
    expected.clear();

    pMsg = error.getSrc(Log::Level::TRACE, __FILE__, line);
    expected << "[ TRACE ] " << __FILE__ << ':' <<  line;
    ASSERT_EQ(expected.str(), pMsg);
    expected.str("");
    expected.clear();
}
