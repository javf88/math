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
set(CMAKE_CXX_STANDARD 17)

## Logging options
set(LOG_LEVEL "TRACE" CACHE STRING "Choose the logging-level, options are: ERROR WARNING INFO DEBUG TRACE")
set(LOG_CONFIG "LOG_LEVEL_${LOG_LEVEL}")
message(STATUS "LOG_CONFIG is ${LOG_CONFIG}")

## Sanitizer options
set(CHECK_TYPE "address" CACHE STRING "Choose the sanitizer, options are: address")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=${CHECK_TYPE}")

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
