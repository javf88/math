#*******************************************************************************
# Define libraries
#*******************************************************************************

# Memory management system
add_library(apple_memory INTERFACE)

target_include_directories(apple_memory
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Logging system
add_library(apple_log INTERFACE)

target_include_directories(apple_log
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(apple_log
    INTERFACE LOG_TO_FILE=$<IF:$<BOOL:${LOG_TO_FILE}>,1U,0U>
    INTERFACE LOG_CONFIG=${LOG_CONFIG})

target_compile_options(apple_log
    INTERFACE -g)

# Matrix algebra
add_library(apple_algebra INTERFACE)

target_include_directories(apple_algebra
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(apple_algebra
    INTERFACE $<TARGET_PROPERTY:apple_log,INTERFACE_COMPILE_DEFINITIONS>)
