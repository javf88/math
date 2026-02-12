#*******************************************************************************
# Define libraries
#*******************************************************************************

# Sorting algorithm
#add_library(memory INTERFACE)

#target_include_directories(memory
#    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

#target_link_libraries(memory
#    INTERFACE log)

# Sorting algorithm
#add_library(sort OBJECT
#    matrix.cpp
#    operators.cpp)
add_library(sort INTERFACE)

target_include_directories(sort
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(sort
    INTERFACE log)
