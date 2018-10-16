add_include_dir (.)

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
    set (AUDIO_LIBS ${PORTAUDIO_LIBRARY} ${JACK_LIBRARY} pthread)
else ()
    list (REMOVE_ITEM AUDIO_SRC_FILES ${WIN_AUDIO_SRC_FILES} ${OSX_AUDIO_SRC_FILES})
    set (AUDIO_LIBS asound pthread)
endif ()

# Generate libraries
add_library (hlaudio STATIC ${AUDIO_SRC_FILES})
set_target_properties (hlaudio PROPERTIES
    VERSION ${VERSION}
    SOVERSION 1
    PUBLIC_HEADER Controller.h
)

target_include_directories (hlaudio PRIVATE . ${OSX_INCLUDES})
install (TARGETS hlaudio
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin                    # DLLs
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib                    # Static libs
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib                    # Shared libs (non-DLL)
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hlaudio  # Headers
    COMPONENT library
)

# Link external libraries to new generated library
target_link_libraries (hlaudio ${AUDIO_LIBS})

set (AUDIO_SRC_FILES ${AUDIO_SRC_FILES} PARENT_SCOPE)
set (AUDIO_LIBS ${AUDIO_LIBS} PARENT_SCOPE)