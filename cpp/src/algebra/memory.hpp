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

template<typename T>
struct Memory: public std::deque<T>
{
    // Do I need this here?
    Log logMemory;

    void clean()
    {
        while (this->empty() == false)
        {
            auto element = this->cbegin();
            LOG_DEBUG(this->logMemory, "Freeing (void*)", *element);
            std::free(*element);
            this->pop_front();
        }

        std::cout << this->logMemory.str();
    }
};

#endif /* MEMORY_H_ */
