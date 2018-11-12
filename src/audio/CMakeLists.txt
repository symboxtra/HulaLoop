set (HL_LIBRARY_NAME "hlaudio")

file (GLOB AUDIO_SRC_FILES *.cpp)
file (GLOB WIN_AUDIO_SRC_FILES Windows*.cpp)
file (GLOB LINUX_AUDIO_SRC_FILES Linux*.cpp)
file (GLOB OSX_AUDIO_SRC_FILES OSX*.cpp)

# Build the daemon for Mac
add_subdirectory(OSXDaemon)

# Find Port Audio and add the common folder so that we can include the ring buffer headers
# Build the ring buffer code into our executable since it isn't exposed on the normal Port Audio library
list (APPEND AUDIO_SRC_FILES ${PROJECT_SOURCE_DIR}/src/libs/portaudio/src/common/pa_ringbuffer.c)
list (APPEND EXTRA_INCLUDES ${PROJECT_SOURCE_DIR}/src/libs/portaudio/include ${PROJECT_SOURCE_DIR}/src/libs/portaudio/src/common)
list (APPEND AUDIO_LIBS portaudio)

# Remove source files that do not pertain to the current OS and pick libraries to link
if (WIN32)
    list (REMOVE_ITEM AUDIO_SRC_FILES ${LINUX_AUDIO_SRC_FILES} ${OSX_AUDIO_SRC_FILES})
    list (APPEND AUDIO_LIBS winmm Ole32)
elseif (OSX)
    list (REMOVE_ITEM AUDIO_SRC_FILES ${LINUX_AUDIO_SRC_FILES} ${WIN_AUDIO_SRC_FILES})

    find_package (JACK REQUIRED)

    # Add the extra include paths and libraries for JACK
    list (APPEND EXTRA_INCLUDES ${JACK_PATH})
    list (APPEND AUDIO_LIBS ${JACK_LIBRARY})
else ()
    list (REMOVE_ITEM AUDIO_SRC_FILES ${WIN_AUDIO_SRC_FILES} ${OSX_AUDIO_SRC_FILES})
    list (APPEND AUDIO_LIBS asound pthread)
endif ()

# Add hlaudio library
include_directories (${EXTRA_INCLUDES})
add_library_target (${HL_LIBRARY_NAME} "${AUDIO_SRC_FILES};${EXTRA_INCLUDES}")

# Force portaudio to build first
add_dependencies (${HL_LIBRARY_NAME} portaudio portaudio_static)

# Link external libraries to new generated library
target_link_libraries (${HL_LIBRARY_NAME} ${AUDIO_LIBS})

# Make these include directories available to the rest of the project
add_include_dir (include)
set (INCLUDE_DIRS ${INCLUDE_DIRS} ${EXTRA_INCLUDES} PARENT_SCOPE)

# Make these libraries available to the rest of the project
set (HL_LIBRARIES ${HL_LIBRARIES} ${HL_LIBRARY_NAME} PARENT_SCOPE)
