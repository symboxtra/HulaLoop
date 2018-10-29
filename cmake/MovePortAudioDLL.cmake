set (PORTAUDIO_DLL ${CMAKE_BINARY_DIR}/bin/portaudio.dll ${CMAKE_BINARY_DIR}/bin/portaudio_x64.dll ${CMAKE_BINARY_DIR}/bin/portaudio_x86.dll)

# Copy PortAudio DLL to bin/test and suppress errors
# Also copy the test.wav file to the bin/test folder
# TODO: Find a better way to reference the project source dir
# ${PROJECT_SOURCE_DIR} does not refer to the root in this cmake file
execute_process (COMMAND ${CMAKE_COMMAND} -E copy ${PORTAUDIO_DLL} ${CMAKE_BINARY_DIR}/../src/test/test.wav ${CMAKE_BINARY_DIR}/bin/test/
        ERROR_QUIET
        OUTPUT_QUIET)