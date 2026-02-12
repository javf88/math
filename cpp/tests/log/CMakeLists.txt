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

# file submodule
add_executable(file
    file.cpp)

target_link_libraries(file
    PRIVATE GTest::gtest_main
    PRIVATE fileInt
    PRIVATE log)
