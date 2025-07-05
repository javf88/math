/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <gtest/gtest.h>
/* TARGET LIBRARY */
#include "memory.hpp"


class DummyData: public testing::Test
{
    private:
        Matrix A;
        uint32_t step = 1;
        uint32_t init = 0;

    public:

        void SetUp() override
        {
            using namespace std;

            cout << "Running SetUp()" << endl;

            for (uint32_t i = 0; i < A.rows; i++)
            {
                for (uint32_t j = 0; j < A.cols; j++)
                {
                    cout << i * A.cols + step << ", ";
                }

                cout << endl;
            }
        }
};

TEST(Matrix, Constructors)
{
    Matrix Empty;
    ASSERT_EQ(0, Empty.rows);
    ASSERT_EQ(0, Empty.cols);
    ASSERT_EQ(true, Empty.val.empty());

    Matrix A(3,5);
    ASSERT_EQ(3, A.rows);
    ASSERT_EQ(5, A.cols);
    ASSERT_EQ(A.rows * A.cols, A.val.capacity());

    Matrix B({1, 2, 3, 4, 5, 6, 7});
    ASSERT_EQ(1, B.rows);
    ASSERT_EQ(7, B.cols);
    ASSERT_EQ(B.rows * B.cols, B.val.capacity());

    using namespace std;
    cout << string(Empty) << endl;
    cout << string(A) << endl;
    cout << string(B) << endl;
}

TEST_F(DummyData, SetUp)
{
    using namespace std;
//    cout << string(this->A) << endl;
}
