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

#include "levels.hpp"

/******************************************************************************/
/*    API                                                                     */
/******************************************************************************/

struct File
{
    const uint32_t toFile = 0U;
    std::fstream file;
    std::string name;

    // Constructors & Destructors
    File(uint32_t flag) : toFile(flag)
    {
        if (this->toFile == 0U)
        {
//          LOG_INFO("Logging only into stderr.");
            std::cerr << "Logging only into stderr." << std::endl;
        }
        else
        {
            std::time_t secs;
            std::time(&secs);
            this->name = "tmp/" + std::to_string(secs) + ".log";

            std::filesystem::create_directory("tmp");
            this->file.open(this->name, std::ios::out);
            if (this->file.is_open())
            {
                this->file << "Logging into cerr and " << this->name << " file." << std::endl;
                std::cerr << "Logging into cerr and " << this->name << " file." << std::endl;
            }
            else
            {
//              LOG_INFO("Logging into stderr and %s file.", "dummy.file");
                std::cerr << "Logging only into cerr." << std::endl;
            }
        }
    }

    ~File()
    {
        std::cerr << "Closing file." << std::endl;
        this->file.close();
    }
};

#endif /* FILE_H_ */
