#*******************************************************************************
# Define tests
#*******************************************************************************

add_library(utilities INTERFACE)
target_include_directories(utilities
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Logging system
if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    # Unit test
    add_subdirectory(log/unit)
    add_subdirectory(algebra/unit)
else() # MATCHES Release
    # Module test
    add_subdirectory(log/module)
    add_subdirectory(algebra/module)
endif()
