#*******************************************************************************
# Define tests for logging module
#*******************************************************************************

# levels submodule
add_executable(levels
    levels.c)

target_link_libraries(levels
    PRIVATE unity::framework
    PRIVATE utilities
    PRIVATE log)

target_compile_definitions(levels
    PRIVATE $<TARGET_PROPERTY:log,COMPILE_DEFINITIONS>)

add_test(NAME levels.UnitTest
    COMMAND levels)

# file submodule
add_executable(file
    file.c)

target_link_libraries(file
    PRIVATE unity::framework
    PRIVATE log)

target_compile_definitions(file
    PRIVATE $<TARGET_PROPERTY:log,COMPILE_DEFINITIONS>)

add_test(NAME file.UnitTest
    COMMAND file)
