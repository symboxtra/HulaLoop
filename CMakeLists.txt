######## Config ########

cmake_minimum_required (VERSION 3.5)

set (CMAKE_CXX_STANDARD 14)

include (cmake/Qt5.cmake)
include (cmake/Utility.cmake)

# Version
set (HL_VERSION_MAJOR 0)
set (HL_VERSION_MINOR 0)
set (HL_VERSION_REV 0)
set (HL_VERSION_BUILD 0)
set (HL_VERSION_STR "${HL_VERSION_MAJOR}.${HL_VERSION_MINOR}.${HL_VERSION_REV}.${HL_VERSION_BUILD}")

# Declare project
project (HulaLoop VERSION ${HL_VERSION_STR} LANGUAGES CXX C)

message (STATUS "")
message (STATUS "Building ${PROJECT_NAME} v${HL_VERSION_STR}")
message (STATUS "")

# Set build type if not provided
# Default to DEBUG
if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Debug)
endif ()

set (OSX FALSE)
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set (OSX TRUE)
endif ()

set (64BIT FALSE)
if (WIN32 AND CMAKE_GENERATOR_PLATFORM MATCHES "x64")
    set (64BIT TRUE)
endif ()

set (HL_PACKAGE_TYPE ".tar.gz")
if (WIN32 AND 64BIT)
    set (HL_PACKAGE_TYPE "win64.exe")
elseif (WIN32 AND NOT 64BIT)
    set (HL_PACKAGE_TYPE "win32.exe")
elseif (OSX)
    set (HL_PACKAGE_TYPE ".dmg")
elseif (EXISTS "/etc/debian_version")
    set (HL_PACKAGE_TYPE ".deb")
elseif (EXISTS "/etc/redhat-release")
    set (HL_PACKAGE_TYPE ".rpm")
endif ()

# Global build options
option (HL_BUILD_ONLY_AUDIO "Build only the audio library." OFF)
option (HL_BUILD_CLI "Build the command line application." ON)
option (HL_BUILD_GUI "Build the graphical application." ON)
option (HL_ADD_COVERAGE_FLAGS "Add coverage options to compiler flags" OFF)

# Disable coverage flags by default
if (CMAKE_BUILD_TYPE MATCHES "Debug" AND (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "AppleClang" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    set (HL_ADD_COVERAGE_FLAGS ON)
endif ()

# Set output for binaries
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/lib)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/lib)
foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string (TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin)
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib/lib)
    set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib/lib)
endforeach ()

# Set output for libs compiled with --target install
# Allow override via -DCMAKE_INSTALL_PREFIX=blah
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND CMAKE_BUILD_TYPE MATCHES "Debug")
    set (CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/lib CACHE PATH "Installation path" FORCE)
    set (CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/lib CACHE PATH "" FORCE) # Should this be here?
endif ()

message (STATUS "System Platform: ${CMAKE_SYSTEM_NAME} ${CMAKE_GENERATOR_PLATFORM}")
message (STATUS "Install Prefix: ${CMAKE_INSTALL_PREFIX}\n")

# Initialize lists
set (INCLUDE_DIRS)
set (HL_LIBRARIES)

# Link Windows specific libraries
if (WIN32)
    set (HL_LIBRARIES ${HL_LIBRARIES} winmm.lib ) # TODO: Add required windows libraries
endif ()

# Make sure we can find our Find<package>.cmake files
list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")




######## Generated Headers ########
configure_file (
    "${PROJECT_SOURCE_DIR}/cmake/HulaVersion.h.in"
    "${CMAKE_BINARY_DIR}/HulaVersion.h"
)
include_directories (${CMAKE_BINARY_DIR})




######## Qt ########

# Prime for Qt
set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

if (WIN32 AND NOT DEFINED QT_ROOT)
    if (DEFINED ENV{QT_ROOT})
        set (QT_ROOT $ENV{QT_ROOT} CACHE STRING "Path to Qt Installation")
    else ()
        message (FATAL_ERROR "QT_ROOT environment variable not set. Add the environment variable in Windows Environment Variables or cmake with -DQT_ROOT=C:\\path\\to\\qt\\5.XX.X")
    endif ()
elseif (WIN32)
    set (QT_ROOT $ENV{QT_ROOT} CACHE STRING "Path to Qt Installation")
endif ()

message (STATUS "QT_ROOT: " ${QT_ROOT} "\n")

if (WIN32 AND NOT DEFINED Qt5_DIR)
    if(64BIT)
        set (Qt5_DIR "${QT_ROOT}\\msvc2017_64\\lib\\cmake\\Qt5")
    else ()
        set (Qt5_DIR "${QT_ROOT}\\msvc2015\\lib\\cmake\\Qt5")
    endif ()
endif ()

message (STATUS "Qt5_DIR: ${Qt5_DIR}")

# Required Qt packages
find_package (Qt5 REQUIRED
    Core
    Gui
    Qml
    Quick
    QuickControls2
    Widgets
)

set (HL_LIBRARIES
    ${HL_LIBRARIES}
    Qt5::Core
    Qt5::Gui
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickControls2
    Qt5::Widgets
)

# Required Sndfile packages
find_package (SndFile REQUIRED)

add_include_dir (${LIBSNDFILE_INCLUDE})
list (APPEND HL_LIBRARIES ${LIBSNDFILE_LIB})

######## Libraries ########

# Setup flags for all external library builds
if (WIN32 AND 64BIT)
    set (HL_EXT_LIB_FLAGS -A x64)
endif ()

# Build and install portaudio to our build/lib folder
add_subdirectory (src/libs/portaudio)

# Add coverage flags for GCC after 3rd party libs are built
set (CXX_FLAGS_NOCOV "${CMAKE_CXX_FLAGS}")
if (HL_ADD_COVERAGE_FLAGS)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif ()

# Add subdirectories and include any headers from lower level CMakeLists
add_subdirectory (src/audio)
include_directories (${INCLUDE_DIRS})

# Build UI and control modules
if (NOT HL_BUILD_ONLY_AUDIO)
    add_subdirectory (src/control)
    include_directories (${INCLUDE_DIRS})

    if (HL_BUILD_CLI)
        add_subdirectory (src/ui/cli)
        include_directories (${INCLUDE_DIRS})
    endif ()

    if (HL_BUILD_GUI)
        add_subdirectory (src/ui/gui)
        add_subdirectory (src/launcher)
        include_directories (${INCLUDE_DIRS})
    endif ()
endif ()




######## Testing ########
# GUI tests are in Testing.cmake
# Updater tests are in Testing.cmake

# Build settings for DEBUG builds only
if (CMAKE_BUILD_TYPE MATCHES "Debug")

    include (cmake/Testing.cmake)

    # Test that only rely on the audio library
    create_test ("src/test/TestOSAudio.cpp" "" 3 FALSE FALSE)
    create_test ("src/test/TestHulaRingBuffer.cpp" "" 0.5 TRUE FALSE)
    create_test ("src/test/TestController.cpp" "" -1 TRUE FALSE)
    create_test ("src/test/TestCLIArgs.cpp" "" -1 FALSE FALSE) # TODO: memcheck can be turned on once Device::id is fixed
    create_test("src/test/TestInteractiveCLI.cpp" "src/ui/cli/InteractiveCLI.cpp" -1 FALSE FALSE)

    if (OSX)
        create_test ("src/test/TestOSXAudio.cpp" "" -1 FALSE FALSE)
    elseif (WIN32)
        create_test ("src/test/TestWindowsAudio.cpp" "" -1 FALSE FALSE)
    endif ()

    if (NOT HL_BUILD_ONLY_AUDIO)
        create_test ("src/test/TestTransport.cpp" "" -1 TRUE FALSE)
        create_test ("src/test/TestRecord.cpp" "" -1 FALSE FALSE)
    endif ()

endif ()

message (STATUS "")
