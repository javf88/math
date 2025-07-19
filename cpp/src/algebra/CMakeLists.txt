#*******************************************************************************
# Define libraries
#*******************************************************************************

# Matrix algebra
add_library(algebra INTERFACE)

target_include_directories(algebra
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
