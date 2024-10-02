#*******************************************************************************
# Define tests
#*******************************************************************************

# memory management submodule
add_executable(memory
    memory.c)

target_link_libraries(memory
    PRIVATE unity::framework
    PRIVATE log
    PRIVATE algebra)

add_test(NAME memory.UnitTest
    COMMAND memory)

# operators submodule
add_executable(operators
    operators.c)

target_link_libraries(operators
    PRIVATE unity::framework
    PRIVATE log
    PRIVATE algebra)

add_test(NAME operators.UnitTest
    COMMAND operators)

# echelon submodule
add_executable(echelon
    echelon.c)

target_link_libraries(echelon
    PRIVATE unity::framework
    PRIVATE log
    PRIVATE algebra)

add_test(NAME echelon.UnitTest
    COMMAND echelon)

# echelon submodule
add_executable(decomposition
    decomposition.c)

target_link_libraries(decomposition
    PRIVATE unity::framework
    PRIVATE log
    PRIVATE algebra)

add_test(NAME decomposition.UnitTest
    COMMAND decomposition)
