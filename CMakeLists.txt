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
option (HL_BUILD_EXAMPLES "Build example programs." OFF)
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




######## Libraries ########

# Setup flags for all external library builds
if (WIN32 AND 64BIT)
    set (HL_EXT_LIB_FLAGS -A x64)
endif ()

# Build and install portaudio to our build/lib folder
add_subdirectory (src/libs/portaudio)

# Make sure we can find our Find<package>.cmake files
list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

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

    if (HL_BUILD_EXAMPLES)
        add_subdirectory (examples)
    endif ()
endif ()

install (
    DIRECTORY ${CMAKE_BINARY_DIR}/bin
    DESTINATION ${CMAKE_INSTALL_PREFIX}
    USE_SOURCE_PERMISSIONS
    PATTERN "*.ilk" EXCLUDE
    PATTERN "*.pdb" EXCLUDE
)




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
    endif ()

endif ()




######## CPack ########
if (NOT DEFINED GEN_INSTALL)
    set (GEN_INSTALL false)
endif ()

if(GEN_INSTALL AND NOT BUILD_ONLY_AUDIO)
    message(STATUS "HERE1")


    set(CPACK_PACKAGE_NAME "HulaLoop Installer")
    set(CPACK_PACKAGE_VENDOR "Symboxtra Software")
    set(CPACK_PACKAGE_VERSION "0.0.0")

    if (64BIT) # Windows x64

        set(CPACK_GENERATOR "NSIS64")

        set(CPACK_NSIS_MUI_ICON "${PROJECT_SOURCE_DIR}/res/logo/hulaloop-logo.ico")
        set(CPACK_NSIS_MUI_UNIICON "${PROJECT_SOURCE_DIR}/res/logo/hulaloop-logo.ico")
#        set(CPACK_PACKAGE_ICON "${PROJECT_SOURCE_DIR}/res/logo/hulaloop-logo.bmp")
        set(CPACK_NSIS_INSTALLED_ICON_NAME "${PROJECT_SOURCE_DIR}/res/logo/hulaloop-logo.ico")
        set(CPACK_PACKAGE_INSTALL_DIRECTORY "Test CPack")
        set(CPACK_NSIS_MODIFY_PATH ON)
        set(CPACK_COMPONENTS_ALL app)

        if(PACK)
            set(INSTALL_DIR bin)
            set(LIB_DIR bin/lib)
        endif()

        include(CPack)

        # Configuration file with custom definitions for NSIS
        # configure_file(
            # /res/nsis/NSIS.definitions.nsh.in
            # ${CMAKE_CURRENT_BINARY_DIR}/res/nsis/NSIS.definitions.nsh
        # )

    elseif (WIN32) # Windows x86

        set(CPACK_GENERATOR "NSIS")

        set(CPACK_NSIS_MUI_ICON "${PROJECT_SOURCE_DIR}/res/logo/hulaloop-logo.ico")
        set(CPACK_NSIS_MUI_UNIICON "${PROJECT_SOURCE_DIR}/res/logo/hulaloop-logo.ico")
#        set(CPACK_PACKAGE_ICON "${PROJECT_SOURCE_DIR}/res/logo/hulaloop-logo.bmp")
        set(CPACK_NSIS_INSTALLED_ICON_NAME "${PROJECT_SOURCE_DIR}/res/logo/hulaloop-logo.ico")
        set(CPACK_PACKAGE_INSTALL_DIRECTORY "Test CPack")
        set(CPACK_NSIS_MODIFY_PATH ON)
        set(CPACK_COMPONENTS_ALL app)

        if(PACK)
            set(INSTALL_DIR bin)
            set(LIB_DIR bin/lib)
        endif()

        include(CPack)

    elseif (OSX) # Mac OS X
        set(CPACK_GENERATOR "DMG")
    else () # Debian Linux
        set(CPACK_GENERATOR "DEB")
        set(CPACK_PACKAGE_VENDOR "Symboxtra")
        set(CPACK_COMPONENTS_ALL exe link configs lastversion script log monitor)
        set(CPACK_PACKAGE_FILE_NAME "HulaLoop")
        set(CPACK_PACKAGE_NAME "HulaLoop")
        set(CPACK_PACKAGE_VERSION 0.0.1)
        set(CPACK_PACKAGE_VERSION_MAJOR 0)
        set(CPACK_PACKAGE_VERSION_MINOR 0)
        set(CPACK_PACKAGE_VERSION_PATCH 1)
        set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
        set(CPACK_PACKAGE_DESCRIPTION "Some Description")
        set(CPACK_PACKAGE_CONTACT "support@company.com")
        install(FILES ${PROJECT_SOURCE_DIR}/res/installer/hulaloop.desktop DESTINATION /usr/share/applications)
        install(FILES ${PROJECT_SOURCE_DIR}/res/logo/hulaloop-logo.svg DESTINATION /usr/share/icons/hicolor/scalable/apps)
        include(CPack)

    endif ()

endif()
message (STATUS "")
