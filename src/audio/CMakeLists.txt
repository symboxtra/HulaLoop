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
elseif (OSX)
    list (REMOVE_ITEM AUDIO_SRC_FILES ${LINUX_AUDIO_SRC_FILES} ${WIN_AUDIO_SRC_FILES})
    list (APPEND AUDIO_SRC_FILES ${OSX_DAEMON_SRC_FILES})
else ()
    list (REMOVE_ITEM AUDIO_SRC_FILES ${WIN_AUDIO_SRC_FILES} ${OSX_AUDIO_SRC_FILES})
endif ()

#TODO: Change this to better apply with the parent CMake
add_executable(test_audio ${AUDIO_SRC_FILES})

# Link OS libraries based on OS
if (WIN32)
    target_link_libraries(test_audio winmm Ole32)
elseif (OSX)
    target_link_libraries(test_audio portaudio jack pthread)# "-framework CoreAudio -framework CoreFoundation")
else ()
    target_link_libraries(test_audio asound pthread)
endif ()