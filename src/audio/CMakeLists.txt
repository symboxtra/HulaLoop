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
if (WIN32)
    list (REMOVE_ITEM AUDIO_SRC_FILES ${LINUX_AUDIO_SRC_FILES} ${OSX_AUDIO_SRC_FILES})
    set (AUDIO_LIBS winmm Ole32)
elseif (OSX)
    list (REMOVE_ITEM AUDIO_SRC_FILES ${LINUX_AUDIO_SRC_FILES} ${WIN_AUDIO_SRC_FILES})
    list (APPEND AUDIO_SRC_FILES ${OSX_DAEMON_SRC_FILES})
    set (AUDIO_LIBS portaudio jack pthread)
else ()
    list (REMOVE_ITEM AUDIO_SRC_FILES ${WIN_AUDIO_SRC_FILES} ${OSX_AUDIO_SRC_FILES})
    set (AUDIO_LIBS asound pthread)
endif ()

# Generate shared libraries
add_library (hlaudio SHARED ${AUDIO_SRC_FILES})
set_target_properties (hlaudio PROPERTIES
    VERSION ${VERSION}
    SOVERSION 1
    PUBLIC_HEADER Controller.h
)
configure_file (hlaudio.pc.in hlaudio.pc @ONLY)
target_include_directories (hlaudio PRIVATE .)
install (TARGETS hlaudio
    DESTINATION ${CMAKE_BINARY_DIR}/bin
    PUBLIC_HEADER DESTINATION ${CMAKE_BINARY_DIR}/include
    COMPONENT library
)
install (FILES ${CMAKE_BINARY_DIR}/bin/hlaudio.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
)

# Link external libraries to new generated library
target_link_libraries (hlaudio ${AUDIO_LIBS})

set (AUDIO_SRC_FILES ${AUDIO_SRC_FILES} PARENT_SCOPE)
set (AUDIO_LIBS ${AUDIO_LIBS} PARENT_SCOPE)
