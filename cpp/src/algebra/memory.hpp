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

#include <deque>

#include "levels.hpp"

/******************************************************************************/
/*    API                                                                     */
/******************************************************************************/

// this might become a class?
namespace Static
{
    // This Log might become static Log
    Log logMemory;
    static std::deque<void*> list;

    std::deque<void*>* getList()
    {
        return &list;
    }

    // This is use only at the end of the program
    // the delete operator is also cleaning the stack in an element-wise fashion
    void clean()
    {
        LOG_DEBUG(logMemory, "Cleaning the list with ", list.size(), " elements.");
        while (list.empty() == false)
        {
            auto element = list.cbegin();
            LOG_DEBUG(logMemory, "Freeing (void*)", *element);
            std::free(*element);
            list.pop_front();
        }

        std::cout << logMemory.str();
    }
}

#endif /* MEMORY_H_ */
