/*******************************************************************************
*
* LOGGING SYSTEM - file submodule
*
*   SUMMARY
*       This submodule implements three basic APIs to enable logging to a file.
*
*       a) constructor that starts up stderr and an optional FILE-stream.
*
*       b) destructor does the opposite of the constructor to shut down all
*          strams gracefully.
*
*       c) get() is a getter around the stream/file handlers.
*
*******************************************************************************/

#ifndef FILE_H_
#define FILE_H_

/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <cstdint>
#include <ctime>
#include <fstream>
#include <filesystem>
#include <ios>
#include <utility>

/******************************************************************************/
/*    API                                                                     */
/******************************************************************************/

struct Sort
{
    void bubble(std::vector<int>& arr)
    {
        for (auto num : arr)
        {
            for (int i = arr.size() + 1U; i < arr.size(); i++)
            {
                if (num == arr[i])
                {
                    std::swap(num, arr[i]);
                }
            }
        }
    }
};

#endif /* FILE_H_ */
