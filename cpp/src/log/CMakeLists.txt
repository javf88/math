#*******************************************************************************
# Define libraries
#*******************************************************************************

# Matrix algebra
add_library(log OBJECT
    levels.cpp)

target_include_directories(log
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(log
    INTERFACE LOG_CONFIG=${LOG_CONFIG})
