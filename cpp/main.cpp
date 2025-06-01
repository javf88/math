#include "memory.hpp"

int main(void)
{
    Matrix A;
    Matrix B(3,5);

    using namespace std;
    cout << string(A) << endl;
    cout << string(B) << endl;
}
