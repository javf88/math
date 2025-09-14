#******************************************************************************U*
# Define tests
#*******************************************************************************

# levels submodule
add_executable(levels
    levels.cpp)

target_link_libraries(levels
    PRIVATE GTest::gtest_main
    PRIVATE log)

gtest_add_tests(TARGET levels)
