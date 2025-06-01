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
