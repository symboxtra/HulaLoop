# Add any DLLs or files that need to be copied to the test directory after build.
# Use all applicable names (i.e. Qt5XXX.dll and Qt5XXXd.dll).
# Any non-existent names will fail silently

set (PORTAUDIO_DLLS ${BINARY_DIR}/bin/portaudio.dll ${BINARY_DIR}/bin/portaudio_x64.dll ${BINARY_DIR}/bin/portaudio_x86.dll)
set (QT_DLLS ${BINARY_DIR}/bin/Qt5Core.dll ${BINARY_DIR}/bin/Qt5Cored.dll)
set (SNDFILE_DLLS ${BINARY_DIR}/bin/libsndfile-1.dll)

# Copy OSXDaemon so that the tests can start it
set (OSX_DAEMON ${BINARY_DIR}/bin/hulaloop-osx-daemon)

# Copy PortAudio DLL to bin/test and suppress errors
# Also copy the test.wav file to the bin/test folder
execute_process (COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PORTAUDIO_DLLS}
        ${QT_DLLS}
        ${SNDFILE_DLLS}
        ${OSX_DAEMON}
        ${SOURCE_DIR}/src/test/test.wav
        ${BINARY_DIR}/bin/test/
        ERROR_QUIET
        OUTPUT_QUIET
)
