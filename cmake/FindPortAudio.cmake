# Find PortAudio
find_library (
    PORTAUDIO_LIBRARY
    NAMES "portaudio"
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
    NAMES portaudio
    NO_DEFAULT_PATH
)

if (NOT PORTAUDIO_LIBRARY)
    message (FATAL_ERROR "Could not find library: PortAudio")
else ()
    message (STATUS "PortAudio Location: ${PORTAUDIO_LIBRARY}")
endif ()

# Find PortAudio includes
find_path (
    PORTAUDIO_PATH
    NAMES "portaudio.h"
    PATHS ${CMAKE_INSTALL_PREFIX}/include
    NO_DEFAULT_PATH
)

if (NOT PORTAUDIO_PATH)
    message (FATAL_ERROR "Could not find PortAudio headers.")
else ()
    message (STATUS "PortAudio Headers: ${PORTAUDIO_PATH}")
endif ()
