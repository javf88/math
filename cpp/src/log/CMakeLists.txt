#*******************************************************************************
# Define libraries
#*******************************************************************************

# Levels
add_library(log OBJECT
    levels.cpp)

target_include_directories(log
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(log
    PUBLIC LOG_CONFIG=${LOG_CONFIG})

# File
# to consolidate afterwards
add_library(fileInt INTERFACE)

target_include_directories(fileInt
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(fileInt
    INTERFACE LOG_TO_FILE=$<IF:$<BOOL:${LOG_TO_FILE}>,1U,0U>)
