add_include_dir (.)

file (GLOB AUDIO_SRC_FILES *.cpp)
file (GLOB WIN_AUDIO_SRC_FILES Windows*.cpp)
file (GLOB LINUX_AUDIO_SRC_FILES Linux*.cpp)
file (GLOB OSX_AUDIO_SRC_FILES OSX*.cpp)

# Remove source files that do not pertain to the current OS
if (WIN32)
    list (REMOVE_ITEM AUDIO_SRC_FILES LinuxAudio.cpp OSXAudio.cpp)
elseif (OSX)
    list (REMOVE_ITEM AUDIO_SRC_FILES ${LINUX_AUDIO_SRC_FILES} ${WIN_AUDIO_SRC_FILES})
else ()
    list (REMOVE_ITEM AUDIO_SRC_FILES ${WIN_AUDIO_SRC_FILES} ${OSX_AUDIO_SRC_FILES})
endif ()

#TODO: Change this to better apply with the parent CMake
add_executable(test_audio ${AUDIO_SRC_FILES})

# Link OS libraries based on OS
if (WIN32)
    target_link_libraries(test_audio winmm Ole32)
elseif (OSX)
    # Setting the linker flag is a hack and shouldn't stick around
    # Couldn't get this to link any other way for the time being
    set (CMAKE_EXE_LINKER_FLAGS "-lportaudio ${CMAKE_EXE_LINKER_FLAGS}")
    target_link_libraries(test_audio portaudio pthread)
else ()
    target_link_libraries(test_audio asound pthread)
endif ()


# Generate shared libraries

add_library (hul1 SHARED
    ${AUDIO_SRC_FILES}
)

set_target_properties(hul1 PROPERTIES
    VERSION ${VERSION}
    SOVERSION 1
    PUBLIC_HEADER Controller.h
)

MESSAGE(STATUS ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
target_include_directories(hul1 PRIVATE .)
install(TARGETS hul1
    LIBRARY DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    ARCHIVE DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    PUBLIC_HEADER DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/includes
    COMPONENT library
)

if(WIN32)
    target_link_libraries(hul1 winmm Ole32)
elseif(OSX)
else()
    target_link_libraries(hul1 asound pthread)
endif()