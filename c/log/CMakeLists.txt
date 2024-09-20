#*******************************************************************************
# Define libraries
#*******************************************************************************

# Logging system
add_library(log INTERFACE)

target_include_directories(log
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(log
    INTERFACE LOG_TO_FILE=$<IF:$<BOOL:${LOG_TO_FILE}>,1U,0U>
    INTERFACE LOG_CONFIG=${LOG_CONFIG})

target_compile_options(log
    INTERFACE -g)
