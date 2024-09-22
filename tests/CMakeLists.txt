#*******************************************************************************
# Define tests
#*******************************************************************************

# Unit test
# Memory management system
add_executable(memory
    memory.c)

target_link_libraries(memory
    PRIVATE unity::framework
    PRIVATE apple_memory)

add_test(NAME memory.UnitTest
    COMMAND memory)

# Logging system
add_subdirectory(log)

# Matrix algebra
add_executable(algebra
    algebra.c)

target_link_libraries(algebra
    PRIVATE unity::framework
    PRIVATE apple_memory
    PRIVATE log
    PRIVATE apple_algebra)

add_test(NAME algebra.UnitTest
    COMMAND algebra)
