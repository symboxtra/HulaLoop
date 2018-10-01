function (create_test _test_file _src_files _timeout)

    get_filename_component (_test_name ${_test_file} NAME_WE)
    add_executable (${_test_name} ${_test_file} ${_src_files})
    target_link_libraries (${_test_name} ${SS_LIBRARIES} gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

    add_test (
        NAME ${_test_name}
        COMMAND ${_test_name} "--gtest_color=yes"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin" #
    )

    if (_timeout GREATER 0)
        set_tests_properties (${_test_name} PROPERTIES TIMEOUT ${_timeout})
    endif ()

endfunction ()

# Prevent overriding of compiler/linker options
if (WIN32)
    set (gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif ()

# Turn off coverage flags
set (CMAKE_CXX_FLAGS "${CXX_FLAGS_NOCOV}")

# Enable testing and build googletest submodule
enable_testing ()
add_subdirectory (src/libs/googletest)

# Add coverage flags
set (CXX_FLAGS_NOCOV "${CMAKE_CXX_FLAGS}")
if (HL_ADD_COVERAGE_FLAGS)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif ()

# Include directories needed for testing
include_directories (
    ${CMAKE_SOURCE_DIR}/src/libs/googletest/googletest/include/gtest
    ${CMAKE_SOURCE_DIR}/src/libs/googletest/googlemock/include/gmock
)

# Let the linker know it should check bin
link_directories (
    ${CMAKE_BINARY_DIR}/bin
)

# Collect all test source files
file (GLOB TEST_SRC_FILES ${CMAKE_SOURCE_DIR}/src/test/*/*.cpp)

set (T_TEST_DIR "${CMAKE_SOURCE_DIR}/src/test")

# Output test executables to bin/test
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test)
foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string (TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin/test)
endforeach ()

