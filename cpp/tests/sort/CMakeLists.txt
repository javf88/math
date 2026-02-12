#******************************************************************************U*
# Define tests
#*******************************************************************************

# sort submodule
add_executable(sort
    sort.cpp)

target_link_libraries(sort
    PRIVATE GTest::gtest_main
    PRIVATE sort
    PRIVATE log)

gtest_add_tests(TARGET sort)
