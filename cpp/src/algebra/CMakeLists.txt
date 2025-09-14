#*******************************************************************************
# Define libraries
#*******************************************************************************

# Memory
add_library(memory INTERFACE)

target_include_directories(memory
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(memory
    INTERFACE log)

# Matrix algebra
add_library(algebra OBJECT
    matrix.cpp
    operators.cpp) # as friend functions

target_include_directories(algebra
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(algebra
    PRIVATE log)
