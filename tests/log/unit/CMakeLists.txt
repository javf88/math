#*******************************************************************************
# Define tests for logging module
#*******************************************************************************

# Forcing LOG_TO_FILE, LOG_CONFIG, LOG_UNIT_TEST macros.
# No need to set them from the buildsystem's UI

# levels submodule
add_executable(levels
    levels.c)

target_link_libraries(levels
    PRIVATE unity::framework
    PRIVATE algebra
    PRIVATE log)

target_compile_definitions(levels
    PRIVATE LOG_TO_FILE=1U
    PRIVATE LOG_CONFIG=LOG_LEVEL_TRACE
    PRIVATE LOG_UNIT_TEST)

add_test(NAME levels.UnitTest
    COMMAND levels)

# file submodule
add_executable(file
    file.c)

target_link_libraries(file
    PRIVATE unity::framework
    PRIVATE log)

target_compile_definitions(file
    PRIVATE LOG_TO_FILE=1U
    PRIVATE LOG_CONFIG=LOG_LEVEL_TRACE
    PRIVATE LOG_UNIT_TEST)

target_compile_options(file
    PRIVATE $<TARGET_PROPERTY:log,COMPILE_OPTIONS>)

add_test(NAME file.UnitTest
    COMMAND file)
