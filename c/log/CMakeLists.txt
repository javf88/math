#*******************************************************************************
# Define libraries
#*******************************************************************************

# Logging system
add_library(log OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/levels.c)

target_include_directories(log
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(log
    PRIVATE LOG_TO_FILE=$<IF:$<BOOL:${LOG_TO_FILE}>,1U,0U>
    PRIVATE LOG_CONFIG=${LOG_CONFIG})
