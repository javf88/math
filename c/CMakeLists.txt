#*******************************************************************************
# Define libraries
#*******************************************************************************

# Memory management system
add_library(apple_memory INTERFACE)

target_include_directories(apple_memory
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Logging system
add_subdirectory(log)

# Matrix algebra
add_subdirectory(algebra)
