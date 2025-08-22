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

#include "levels.hpp"

/******************************************************************************/
/*    API                                                                     */
/******************************************************************************/

namespace Static
{
    Log logMemory;
    // std::stack has pop() and push() capabilities already.
    // I might need to repalce the stack with a deque
    static std::stack<void*> stack;

    std::stack<void*>* getStack()
    {
        return &stack;
    }

    void clean()
    {
        LOG_DEBUG(logMemory, "Cleaning the stack with ", stack.size(), " elements.");
        while (stack.empty() == false)
        {
            stack.pop();
        }
    }
}

#endif /* MEMORY_H_ */
