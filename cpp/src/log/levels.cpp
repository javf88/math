/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <ostream>
#include <iostream>

#include "levels.hpp"

/******************************************************************************/
/*    IMPLEMENTATION                                                          */
/******************************************************************************/

void Log::log()
{
    *this << Log::MSG::ENDL;
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
        "\x1b[37m",   //White
        "\x1b[0m",    //Disable coloring
        "\n"
    };

    os << color[fmt];
    return os;
}
