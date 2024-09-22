#*******************************************************************************
# Define tests
#*******************************************************************************

# operators submodule
add_executable(operators
    operators.c)

target_link_libraries(operators
    PRIVATE unity::framework
    PRIVATE apple_memory
    PRIVATE log
    PRIVATE algebra)

add_test(NAME operators.UnitTest
    COMMAND operators)
