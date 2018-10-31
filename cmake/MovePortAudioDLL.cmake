set (PORTAUDIO_DLL ${BINARY_DIR}/bin/portaudio.dll ${BINARY_DIR}/bin/portaudio_x64.dll ${BINARY_DIR}/bin/portaudio_x86.dll)

# Copy PortAudio DLL to bin/test and suppress errors
# Also copy the test.wav file to the bin/test folder
execute_process (COMMAND ${CMAKE_COMMAND} -E copy
        ${PORTAUDIO_DLL} ${SOURCE_DIR}/src/test/test.wav
        ${BINARY_DIR}/bin/test/
        ERROR_QUIET
        OUTPUT_QUIET
)
