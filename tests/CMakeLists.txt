#*******************************************************************************
# Define tests
#*******************************************************************************

# Logging system
if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    # Unit test
    add_subdirectory(log/unit)
else() # MATCHES Release
    # Module test
    add_subdirectory(log/module)
endif()

# Matrix algebra
add_subdirectory(algebra)
