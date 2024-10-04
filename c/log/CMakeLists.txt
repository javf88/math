#*******************************************************************************
# Define libraries
#*******************************************************************************

# Logging module
add_library(log OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/levels.c
    ${CMAKE_CURRENT_SOURCE_DIR}/file.c)

target_include_directories(log
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(log
    PRIVATE LOG_TO_FILE=$<IF:$<BOOL:${LOG_TO_FILE}>,1U,0U>
    PRIVATE LOG_CONFIG=${LOG_CONFIG}
    # For now we keep this macro, I need to consolidate for advanced use
    PRIVATE LOG_UNIT_TEST)
