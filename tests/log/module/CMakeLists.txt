#*******************************************************************************
# Define tests for logging module
#*******************************************************************************

# log module
add_executable(logModule
    log.c)

target_link_libraries(logModule
    PRIVATE unity::framework
    PRIVATE algebra
    PRIVATE log)

target_compile_definitions(logModule
    PRIVATE $<TARGET_PROPERTY:log,COMPILE_DEFINITIONS>)

add_test(NAME log.ModuleTest
    COMMAND logModule)
