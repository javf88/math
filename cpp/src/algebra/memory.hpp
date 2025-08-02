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
    Log LogMemory;
    // std::stack has pop() and push() capabilities already.
    static std::stack<void*> stack;

    std::stack<void*>* getStack()
    {
        return &stack;
    }

    void clean()
    {
        LOG_DEBUG(LogMemory, "Cleaning the stack with ", stack.size(), " elements.");
        while (stack.empty() == false)
        {
            stack.pop();
        }
    }
}

#endif /* MEMORY_H_ */
