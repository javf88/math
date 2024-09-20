#*******************************************************************************
# Define tests
#*******************************************************************************

# levels submodule
add_executable(levels
    levels.c)

target_link_libraries(levels
    PRIVATE unity::framework
    PRIVATE apple_log)

add_test(NAME levels.UnitTest
    COMMAND levels)

# file submodule
add_executable(file
    file.c)

target_link_libraries(file
    PRIVATE unity::framework
    PRIVATE apple_log)

add_test(NAME file.UnitTest
    COMMAND file)
