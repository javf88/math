#*******************************************************************************
# Define tests
#*******************************************************************************

# Unit test
# Logging system
add_executable(levels
    levels.c)

target_link_libraries(levels
    PRIVATE unity::framework
    PRIVATE apple_log)


add_test(NAME levels.UnitTest
    COMMAND levels)
