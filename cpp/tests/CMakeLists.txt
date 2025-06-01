#*******************************************************************************
# Define tests
#*******************************************************************************

# memory management submodule
add_executable(memory
    memory.cpp)

target_link_libraries(memory
    PRIVATE algebra)

#add_test(NAME memory.UnitTest
#    COMMAND memory)
