#*******************************************************************************
# Define libraries
#*******************************************************************************

# Memory management system
add_library(apple_memory INTERFACE)

target_include_directories(apple_memory
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Matrix algebra
add_library(apple_algebra INTERFACE)

target_include_directories(apple_algebra
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
