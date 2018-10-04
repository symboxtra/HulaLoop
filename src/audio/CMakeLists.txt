add_include_dir (.)

file (GLOB AUDIO_SRC_FILES *.cpp)
file (GLOB WIN_AUDIO_SRC_FILES Windows*.cpp)
file (GLOB LINUX_AUDIO_SRC_FILES Linux*.cpp)
file (GLOB OSX_AUDIO_SRC_FILES OSX*.cpp)

# Remove source files that do not pertain to the current OS
if (WIN32)
    list (REMOVE_ITEM AUDIO_SRC_FILES ${LINUX_AUDIO_SRC_FILES} ${OSX_AUDIO_SRC_FILES})
    set (AUDIO_LIBS winmm Ole32)
elseif (OSX)
    list (REMOVE_ITEM AUDIO_SRC_FILES ${LINUX_AUDIO_SRC_FILES} ${WIN_AUDIO_SRC_FILES})
    set (AUDIO_LIBS portaudio pthread)
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
    PUBLIC_HEADER DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/include
    COMPONENT library
)
install (FILES ${CMAKE_BINARY_DIR}/bin/hlaudio.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
)

#TODO: Remove after lib integration with UI is successfull
add_executable(test_audio ${AUDIO_SRC_FILES})

# Link external libraries to test executable and new generated library
target_link_libraries (hlaudio ${AUDIO_LIBS})
target_link_libraries (test_audio ${AUDIO_LIBS})

if (OSX)
    # Setting the linker flag is a hack and shouldn't stick around
    # Couldn't get this to link any other way for the time being
    set (CMAKE_EXE_LINKER_FLAGS "-lportaudio ${CMAKE_EXE_LINKER_FLAGS}")
    target_link_libraries(test_audio portaudio pthread)
    target_link_libraries(hlaudio portaudio pthread)
endif ()