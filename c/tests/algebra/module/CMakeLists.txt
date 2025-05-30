#*******************************************************************************
# Define tests
#*******************************************************************************

# echelon submodule
add_executable(echelon
    echelon.c)

target_link_libraries(echelon
    PRIVATE unity::framework
    PRIVATE utilities
    PRIVATE log
    PRIVATE algebra)

target_compile_definitions(echelon
    PRIVATE $<TARGET_PROPERTY:log,COMPILE_DEFINITIONS>)

add_test(NAME echelon.UnitTest
    COMMAND echelon)
