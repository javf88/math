/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <gtest/gtest.h>
/* TARGET LIBRARY */
#include "memory.hpp"

TEST(Matrix, Constructors)
{
    Matrix A;
    Matrix B(3,5);

    using namespace std;
    cout << string(A) << endl;
    cout << string(B) << endl;
}
