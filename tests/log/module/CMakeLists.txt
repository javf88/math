#*******************************************************************************
# Define tests for logging module
#*******************************************************************************

# Forcing LOG_TO_FILE, LOG_CONFIG, LOG_UNIT_TEST macros.
# No need to set them from the buildsystem's UI

# log module
add_executable(logModule
    log.c)

target_link_libraries(logModule
    PRIVATE unity::framework
    PRIVATE algebra
    PRIVATE log)

target_compile_definitions(logModule
    PRIVATE LOG_TO_FILE=1U
    PRIVATE LOG_CONFIG=LOG_LEVEL_TRACE
    PRIVATE LOG_UNIT_TEST)

add_test(NAME log.ModuleTest
    COMMAND logModule)
