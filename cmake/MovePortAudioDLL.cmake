set (PORTAUDIO_DLL ${CMAKE_BINARY_DIR}/bin/portaudio.dll ${CMAKE_BINARY_DIR}/bin/portaudio_x64.dll ${CMAKE_BINARY_DIR}/bin/portaudio_x86.dll)

# Copy PortAudio DLL to bin/test and suppress errors
execute_process (COMMAND ${CMAKE_COMMAND} -E copy ${PORTAUDIO_DLL} ${CMAKE_BINARY_DIR}/bin/test/
        ERROR_QUIET
        OUTPUT_QUIET)