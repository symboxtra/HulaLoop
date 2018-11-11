######## Config ########

cmake_minimum_required (VERSION 3.5)

set (CMAKE_CXX_STANDARD 14)

# Use version string from HulaLoop if defined
if (NOT HL_VERSION_STR)
    set (HL_VERSION_STR "0.0.3")
endif ()

# Declare project
project (HulaLoop-OSX-Daemon VERSION ${HL_VERSION_STR} LANGUAGES CXX C)

# Make sure we can find our Find<package>.cmake files
# This is only needed for standalone builds as HulaLoop will already
# have provided it on normal builds
if (CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    get_filename_component (HL_MODULE_PATH "../../../cmake" REALPATH)
    message (STATUS "Module path expanded to: ${HL_MODULE_PATH}")

    list (APPEND CMAKE_MODULE_PATH ${HL_MODULE_PATH})
endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

    find_package (Qt5 REQUIRED
        Core
    )

    if (NOT QT_TRANSLATION_LIBS)
        # Minimum set of libs required
        # for translation
        set (QT_TRANSLATION_LIBS
            Qt5::Core
        )
    endif ()

    find_package (JACK REQUIRED)

    file (GLOB OSX_DAEMON_SRC_FILES *.cpp)

    include_directories (${JACK_PATH})
    include_directories ("../include")

    add_executable (hulaloop-osx-daemon ${OSX_DAEMON_SRC_FILES})
    target_link_libraries (hulaloop-osx-daemon ${JACK_LIBRARY} ${QT_TRANSLATION_LIBS})

endif ()