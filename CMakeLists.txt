######## Config ########

cmake_minimum_required (VERSION 3.5)
set (CMAKE_CXX_STANDARD 11)

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
# TODO: Coverage flags

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
    Widgets
    Gui
)

set (HL_LIBRARIES
    ${HL_LIBRARIES}
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
)



######## Libraries ########

if (OSX)
    # Build PortAudio
    add_subdirectory (${CMAKE_SOURCE_DIR}/src/libs/portaudio)

    # Find what we built
    find_library (
        PORTAUDIO_LIB
        NAMES portaudio
        HINTS ${CMAKE_BINARY_DIR}/lib/lib
    )

    if (NOT PORTAUDIO_LIB)
	    message (FATAL_ERROR "Could not find library: PortAudio")
    else ()
        message (STATUS "PortAudio Location: ${PORTAUDIO_LIB}")
    endif()

    # Add the include path
    include_directories(${CMAKE_SOURCE_DIR}/src/libs/portaudio/include)
endif ()

add_subdirectory (src/audio)

# Include any directories that lower level CMakeLists added
include_directories (${INCLUDE_DIRS})


######## Executable ########

# TODO: Add coverage flags for GCC

# TODO: Add final executale here




######## Testing ########

# Build settings for DEBUG builds only
if (CMAKE_BUILD_TYPE MATCHES "Debug")

    # Remove after googletest submodule has been added
    #include (cmake/Testing.cmake)

    # Add more tests here using create_test()

endif ()
