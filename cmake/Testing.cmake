function (create_test _test_file _src_files _timeout _do_memcheck _only_master)

    get_filename_component (_test_name ${_test_file} NAME_WE)
    add_executable (${_test_name} ${_test_file} ${_src_files})
    target_link_libraries (${_test_name} ${HL_LIBRARIES} gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

    # Make sure the real application builds first
    if (NOT HL_BUILD_ONLY_AUDIO)
        add_dependencies (${_test_name} hulaloop hulaloop-cli hulaloop-launcher)
        if (OSX)
            add_dependencies (${_test_name} hulaloop-osx-daemon)
        endif ()
    endif ()

    if (_only_master)
        add_test (
            NAME only_master_${_test_name}
            COMMAND ${_test_name} "--gtest_color=yes"
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )
    else ()
        add_test (
            NAME ${_test_name}
            COMMAND ${_test_name} "--gtest_color=yes"
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )
    endif ()

    # Add memory test
    if (_do_memcheck AND NOT WIN32 AND NOT OSX)
        if (NOT VALGRIND_EXECUTABLE)
            message (STATUS "Valgrind could not be found. Skipping requested memcheck for ${_test_name}.")
        else ()
            add_test (
                NAME only_master_memcheck_${_test_name}
                COMMAND ${VALGRIND_EXECUTABLE} --leak-check=full --error-exitcode=1 --track-origins=yes $<TARGET_FILE:${_test_name}>
                WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
            )
        endif ()
    endif ()

    # Don't apply a timeout to the memory checks since valgrind will slow things down
    if (_timeout GREATER 0)
        set_tests_properties (${_test_name} PROPERTIES TIMEOUT ${_timeout})
    endif ()

    # Copy PortAudio DLLs to bin/test for successful testing
    add_custom_command (
        TARGET ${_test_name}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -DSOURCE_DIR="${PROJECT_SOURCE_DIR}" -DBINARY_DIR="${CMAKE_BINARY_DIR}" -P ${PROJECT_SOURCE_DIR}/cmake/MoveFilesToTest.cmake
    )

endfunction ()

# Make gui tests opt-out
# These tests can't be run on headless platforms
if (NOT DEFINED HL_INCLUDE_GUI_TESTS)
    set (HL_INCLUDE_GUI_TESTS true)
endif ()

# Prevent overriding of compiler/linker options
if (WIN32)
    set (gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif ()

# Turn off coverage flags
set (CMAKE_CXX_FLAGS "${CXX_FLAGS_NOCOV}")

# Enable testing and build googletest submodule
include (CTest)
enable_testing ()
add_subdirectory (src/libs/googletest)

# Add coverage flags
set (CXX_FLAGS_NOCOV "${CMAKE_CXX_FLAGS}")
if (HL_ADD_COVERAGE_FLAGS)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif ()

# Include directories needed for testing
include_directories (
    ${PROJECT_SOURCE_DIR}/src/libs/googletest/googletest/include/gtest
    ${PROJECT_SOURCE_DIR}/src/libs/googletest/googlemock/include/gmock
)

# Let the linker know it should check bin
link_directories (
    ${CMAKE_BINARY_DIR}/bin
)

# Find valgrind for memcheck tests
# This is not required; memchecks will just be skipped if not present
find_program (VALGRIND_EXECUTABLE "valgrind")
message (STATUS "Valgrind: ${VALGRIND_EXECUTABLE}")

# Collect all test source files
file (GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/src/test/*/*.cpp)

set (T_TEST_DIR "${PROJECT_SOURCE_DIR}/src/test")

# Add the GUI tests to the bin directory instead of bin/test
# GUI tests need the Qt DLLs in bin
if (HL_BUILD_GUI AND HL_INCLUDE_GUI_TESTS AND NOT HL_BUILD_ONLY_AUDIO)
    create_test ("src/test/TestGUI.cpp" "src/ui/gui/QMLBridge.cpp;src/ui/gui/SystemTrayIcon.cpp;src/ui/gui/qml.qrc" -1 FALSE FALSE)
    create_test ("src/test/TestUpdater.cpp" "src/launcher/Updater.cpp;src/ui/gui/qml.qrc" -1 FALSE FALSE)
else (NOT HL_INCLUDE_GUI_TESTS)
    message (STATUS "Ignoring GUI tests. Set HL_INCLUDE_GUI_TESTS=true to include.")
endif ()

# Output test executables to bin/test
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test)
foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string (TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin/test)
endforeach ()

