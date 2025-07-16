#*******************************************************************************
# Project config
#*******************************************************************************/

cmake_minimum_required(VERSION 3.20)

project(math
    LANGUAGES CXX
    DESCRIPTION "Some math operators on apple M1")

# Includes
include(CTest)

# Build options
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON" CACHE BOOL "Export commands for lsp" FORCE)
set(CMAKE_CXX_STANDARD 14)

#*******************************************************************************
# CMake Submodules
#*******************************************************************************

# include(GoogleTest) is not working
find_package(GTest REQUIRED)

#*******************************************************************************
# Implementation
#*******************************************************************************

add_subdirectory(src)

#*******************************************************************************
# Testing
#*******************************************************************************

add_subdirectory(tests)
