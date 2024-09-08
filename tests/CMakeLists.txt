#*******************************************************************************
# Define tests
#*******************************************************************************

add_executable(memory
    memory.c)

target_link_libraries(memory
    PRIVATE unity::framework
    PRIVATE apple_memory)

add_test(NAME memory.UnitTest
    COMMAND memory)
