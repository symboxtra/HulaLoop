add_include_dir (.)

file (GLOB AUDIO_SRC_FILES *.cpp)
file (GLOB WIN_AUDIO_SRC_FILES Windows*.cpp)
file (GLOB LINUX_AUDIO_SRC_FILES Linux*.cpp)
file (GLOB OSX_AUDIO_SRC_FILES OSX*.cpp)

# Remove source files that do not pertain to the current OS
if (WIN32)
    list (REMOVE_ITEM AUDIO_SRC_FILES ${LINUX_AUDIO_SRC_FILES} ${OSX_AUDIO_SRC_FILES})
elseif (OSX)
    list (REMOVE_ITEM AUDIO_SRC_FILES ${LINUX_AUDIO_SRC_FILES} ${WIN_AUDIO_SRC_FILES})
else ()
    list (REMOVE_ITEM AUDIO_SRC_FILES ${WIN_AUDIO_SRC_FILES} ${OSX_AUDIO_SRC_FILES})
endif ()

# Link OS libraries based on OS
if (WIN32)
    set (AUDIO_LIBS winmm Ole32)
elseif (OSX)
    set (AUDIO_LIBS portaudio pthread)
else ()
    set (AUDIO_LIBS asound pthread)
endif ()

# Make audio source and libs available in parent scope
set (AUDIO_SRC_FILES ${AUDIO_SRC_FILES} PARENT_SCOPE)
set (AUDIO_LIBS ${AUDIO_LIBS} PARENT_SCOPE)