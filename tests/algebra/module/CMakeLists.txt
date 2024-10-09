#*******************************************************************************
# Define tests
#*******************************************************************************

# echelon submodule
add_executable(echelon
    echelon.c)

target_link_libraries(echelon
    PRIVATE unity::framework
    PRIVATE log
    PRIVATE algebra)

add_test(NAME echelon.UnitTest
    COMMAND echelon)
