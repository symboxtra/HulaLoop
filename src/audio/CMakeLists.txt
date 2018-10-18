set (HL_LIBRARY_NAME "hlaudio")

file (GLOB AUDIO_SRC_FILES *.cpp)
file (GLOB WIN_AUDIO_SRC_FILES Windows*.cpp)
file (GLOB LINUX_AUDIO_SRC_FILES Linux*.cpp)
file (GLOB OSX_AUDIO_SRC_FILES OSX*.cpp)

# Collect source files for the daemon (remove standalone main.cpp)
file (GLOB OSX_DAEMON_SRC_FILES OSXDaemon/*.cpp)
get_filename_component(DAEMON_MAIN OSXDaemon/main.cpp REALPATH)
list (REMOVE_ITEM OSX_DAEMON_SRC_FILES ${DAEMON_MAIN})

# Remove source files that do not pertain to the current OS
# Link OS libraries based on OS
if (WIN32)
    list (REMOVE_ITEM AUDIO_SRC_FILES ${LINUX_AUDIO_SRC_FILES} ${OSX_AUDIO_SRC_FILES})
    set (AUDIO_LIBS winmm Ole32)
elseif (OSX)
    list (REMOVE_ITEM AUDIO_SRC_FILES ${LINUX_AUDIO_SRC_FILES} ${WIN_AUDIO_SRC_FILES})
    list (APPEND AUDIO_SRC_FILES ${OSX_DAEMON_SRC_FILES})

    find_package (PortAudio REQUIRED)
    find_package (JACK REQUIRED)

    # Add the extra include paths and libraries for PortAudio and JACK
    # Include direct link to PortAudio src/common since pa_ringbuffer isn't exposed
    set (OSX_INCLUDES ${PORTAUDIO_PATH} ${JACK_PATH} ${PROJECT_SOURCE_DIR}/src/libs/portaudio/src/common)
    set (AUDIO_LIBS ${PORTAUDIO_LIBRARY} ${JACK_LIBRARY})
else ()
    list (REMOVE_ITEM AUDIO_SRC_FILES ${WIN_AUDIO_SRC_FILES} ${OSX_AUDIO_SRC_FILES})
    set (AUDIO_LIBS asound pthread)
endif ()

# Add library
add_library_target (${HL_LIBRARY_NAME} "${AUDIO_SRC_FILES}")
add_include_dir (include)

# Link external libraries to new generated library
target_link_libraries (${HL_LIBRARY_NAME} ${AUDIO_LIBS})

set (AUDIO_SRC_FILES ${AUDIO_SRC_FILES} PARENT_SCOPE)
set (HL_LIBRARIES ${HL_LIBRARIES} ${HL_LIBRARY_NAME} PARENT_SCOPE)