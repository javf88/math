#*******************************************************************************
# Define tests
#*******************************************************************************

# memory management submodule
add_executable(memory
    memory.c)

target_link_libraries(memory
    PRIVATE unity::framework
    PRIVATE utilities
    PRIVATE log
    PRIVATE algebra)

target_compile_definitions(memory
    PRIVATE $<TARGET_PROPERTY:log,COMPILE_DEFINITIONS>)

add_test(NAME memory.UnitTest
    COMMAND memory)

# operators submodule
add_executable(operators
    operators.c)

target_link_libraries(operators
    PRIVATE unity::framework
    PRIVATE utilities
    PRIVATE log
    PRIVATE algebra)

target_compile_definitions(operators
    PRIVATE $<TARGET_PROPERTY:log,COMPILE_DEFINITIONS>)

add_test(NAME operators.UnitTest
    COMMAND operators)
