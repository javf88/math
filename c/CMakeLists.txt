#*******************************************************************************
# Define libraries
#*******************************************************************************

add_library(apple_memory INTERFACE)

target_include_directories(apple_memory
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
