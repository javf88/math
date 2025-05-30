#*******************************************************************************
# Project config
#*******************************************************************************/

cmake_minimum_required(VERSION 3.20)

project(math
    LANGUAGES CXX
    DESCRIPTION "Some math operators on apple M1")

# initial file
add_executable(initial main.cpp)
