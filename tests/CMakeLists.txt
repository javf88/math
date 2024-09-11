#*******************************************************************************
# Define tests
#*******************************************************************************

# Memory management system
add_executable(memory
    memory.c)

target_link_libraries(memory
    PRIVATE unity::framework
    PRIVATE apple_memory)

add_test(NAME memory.UnitTest
    COMMAND memory)

# Logging system
add_executable(log
    log.c)

target_link_libraries(log
    PRIVATE unity::framework
    PRIVATE apple_log)


add_test(NAME log.UnitTest
    COMMAND log)
