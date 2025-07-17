#*******************************************************************************
# Define tests
#*******************************************************************************

# memory management submodule
add_executable(memory
    memory.cpp)

target_link_libraries(memory
    PRIVATE GTest::gtest_main
    PRIVATE algebra)

gtest_add_tests(TARGET memory)

# matrix submodule
add_executable(matrix
    matrix.cpp)

target_link_libraries(matrix
    PRIVATE GTest::gtest_main
    PRIVATE algebra)

gtest_add_tests(TARGET matrix)

# operators submodule
add_executable(operators
    operators.cpp)

target_link_libraries(operators
    PRIVATE GTest::gtest_main
    PRIVATE algebra)

gtest_add_tests(TARGET operators)
