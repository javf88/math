#*******************************************************************************
# Define tests
#*******************************************************************************

# levels submodule
add_executable(levels
    static.c)

target_link_libraries(levels
    PRIVATE unity::framework
    PRIVATE algebra)

# the next two target_* are consequence of #include "levels.c"
target_include_directories(levels
    PRIVATE $<TARGET_PROPERTY:log,INTERFACE_INCLUDE_DIRECTORIES>)

target_compile_definitions(levels
    PRIVATE $<TARGET_PROPERTY:log,COMPILE_DEFINITIONS>)

add_test(NAME levels.UnitTest
    COMMAND levels)

# file submodule
add_executable(file
    file.c)

target_link_libraries(file
    PRIVATE unity::framework
    PRIVATE log)

target_compile_definitions(file
    PRIVATE LOG_UNIT_TEST)

add_test(NAME file.UnitTest
    COMMAND file)

# log module
add_executable(logModule
    log.c)

target_link_libraries(logModule
    PRIVATE unity::framework
    PRIVATE algebra
    PRIVATE log)

add_test(NAME log.ModuleTest
    COMMAND logModule)
