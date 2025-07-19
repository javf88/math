/*******************************************************************************
*
* MEMORY MANAGEMENT SYSTEM
*
*   SUMMARY
*       A simple static stack to bookkeep the pointers of all dynamically
*       allocated memory with the matrix.
*
*******************************************************************************/

#ifndef MEMORY_H_
#define MEMORY_H_

/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <stack>

/******************************************************************************/
/*    API                                                                     */
/******************************************************************************/

namespace Static
{
    // std::stack has pop() and push() capabilities already.
    static std::stack<void*> stack;

    std::stack<void*>* getStack()
    {
        return &stack;
    }

    void clean()
    {
        while (stack.empty() == false)
        {
            stack.pop();
        }
    }
}

#endif /* MEMORY_H_ */
