#*******************************************************************************
# Project config
#*******************************************************************************/

cmake_minimum_required(VERSION 3.20)

project(math
    LANGUAGES CXX
    DESCRIPTION "Some math operators on apple M1")

include(CTest)
# include(GoogleTest) is not working
find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 11)

#*******************************************************************************
# Implementation
#*******************************************************************************

add_subdirectory(src)

#*******************************************************************************
# Testing
#*******************************************************************************

add_subdirectory(tests)
