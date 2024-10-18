#*******************************************************************************
# Project config
#*******************************************************************************/

cmake_minimum_required(VERSION 3.20)
# Forcing CMAKE_BUILD_TYPE=Debug as initial value
set(ENV{CMAKE_BUILD_TYPE} Debug)

project(M1_math
    LANGUAGES C
    DESCRIPTION "Some math operators on apple M1")

# Includes
include(CTest)

# Build options
message(STATUS "Forcing CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON" CACHE BOOL "Export commands for lsp" FORCE)

set(LOG_TO_FILE "ON" CACHE BOOL "Logging to file is active by defult")
message(STATUS "LOG_TO_FILE is ${LOG_TO_FILE}")

set(LOG_LEVEL "TRACE" CACHE STRING "Choose the logging-level, options are: ERROR WARNING INFO DEBUG TRACE")
set(LOG_CONFIG "LOG_LEVEL_${LOG_LEVEL}")
message(STATUS "LOG_CONFIG is ${LOG_CONFIG}")

#*******************************************************************************
# CMake Submodules
#*******************************************************************************

find_package(Unity 2.6 REQUIRED)
if (Unity_FOUND)
    message(STATUS "Unity has been found")
endif()

#*******************************************************************************
# Implementation
#*******************************************************************************

add_subdirectory(src)

#*******************************************************************************
# Testing
#*******************************************************************************

add_subdirectory(tests)
