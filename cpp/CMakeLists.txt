#*******************************************************************************
# Project config
#*******************************************************************************/

cmake_minimum_required(VERSION 3.20)

project(math
    LANGUAGES CXX
    DESCRIPTION "Some math operators on apple M1")

find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 11)

# initial file
add_library(algebra INTERFACE)

target_include_directories(algebra
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(initial main.cpp)

target_link_libraries(initial
    INTERFACE algebra)
