######## Config ########

cmake_minimum_required (VERSION 3.5)

set (CMAKE_CXX_STANDARD 14)

include (cmake/Qt5.cmake)
include (cmake/Utility.cmake)

# Version
set (VERSION_MAJOR 0)
set (VERSION_MINOR 0)
set (VERSION_REV 0)
set (VERSION_BUILD 0)
set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_REV}.${VERSION_BUILD}")

# Declare project
project (HulaLoop VERSION ${VERSION} LANGUAGES CXX)

message (STATUS "")
message (STATUS "Building ${PROJECT_NAME} v${VERSION}")
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

# Disable coverage flags by default
option (HL_ADD_COVERAGE_FLAGS "Add coverage options to compiler flags" OFF)
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
set (CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/lib)

message (STATUS "System Platform: ${CMAKE_SYSTEM_NAME} ${CMAKE_GENERATOR_PLATFORM}\n")

# Initialize lists
set (SRC_FILES)
set (INCLUDE_DIRS)
set (HL_LIBRARIES)

# Link Windows specific libraries
if (WIN32)
    set (HL_LIBRARIES ${HL_LIBRARIES} winmm.lib ) # TODO: Add required windows libraries
endif ()




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
elseif (NOT DEFINED QT_ROOT)
    set (QT_ROOT $ENV{QT_ROOT} CACHE STRING "Path to Qt Installation")
endif ()

message (STATUS "QT_ROOT: " ${QT_ROOT} "\n")

if (WIN32 AND NOT DEFINED Qt5_DIR)
    if(64BIT)
        set (Qt5_DIR "${QT_ROOT}\\msvc2017_64\\lib\\cmake\\Qt5")
    else ()
        set (Qt5_DIR "${QT_ROOT}\\msvc2015\\lib\\cmake\\Qt5")
    endif ()

elseif (NOT DEFINED Qt5_DIR)
    set (Qt5_DIR "${QT_ROOT}/gcc_64/lib/cmake/Qt5")
endif ()

message (STATUS "Qt5_DIR: ${Qt5_DIR}")

# Required Qt packages
find_package (Qt5 REQUIRED
    Core
    Gui
    Qml
    Quick
    QuickControls2
)

set (HL_LIBRARIES
    ${HL_LIBRARIES}
    Qt5::Core
    Qt5::Gui
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickControls2
)



######## Libraries ########

if (OSX)
    # Find PortAudio
    find_library (
        PORTAUDIO_LIB
        NAMES portaudio
    )

    if (NOT PORTAUDIO_LIB)
        message (FATAL_ERROR "Could not find library: PortAudio")
    else ()
        message (STATUS "PortAudio Location: ${PORTAUDIO_LIB}")
    endif()
endif ()

# Add coverage flags for GCC after 3rd party libs are built
set (CXX_FLAGS_NOCOV "${CMAKE_CXX_FLAGS}")
if (HL_ADD_COVERAGE_FLAGS)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif ()

# Determine whether the UIs and control portions should be built
if (NOT DEFINED BUILD_ONLY_AUDIO)
    set (BUILD_ONLY_AUDIO false)
endif ()

# Add subdirectories and include any headers from lower level CMakeLists
add_subdirectory (src/audio)
include_directories (${INCLUDE_DIRS})
set (HL_LIBRARIES ${HL_LIBRARIES} hlaudio)

# Build UI and control modules
if (NOT BUILD_ONLY_AUDIO)
    add_subdirectory (src/control)
    include_directories (${INCLUDE_DIRS})

    add_subdirectory (src/ui)
    include_directories (${INCLUDE_DIRS})
endif ()




######## Testing ########
# GUI tests are in Testing.cmake

# Build settings for DEBUG builds only
if (CMAKE_BUILD_TYPE MATCHES "Debug")

    include (cmake/Testing.cmake)

    # Test that only rely on the audio library
    create_test("src/test/TestController.cpp" "" -1)
    #create_test("src/test/TestAudioOutput.cpp" "" -1)

    if (NOT BUILD_ONLY_AUDIO)
        create_test("src/test/TestCTInterface.cpp" "src/control/transport.cpp" -1)
    endif ()

endif ()




######## CPack ########
if (NOT DEFINED GEN_INSTALL)
    set (GEN_INSTALL false)
endif ()

if(GEN_INSTALL AND NOT BUILD_ONLY_AUDIO)

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
    endif ()

endif()