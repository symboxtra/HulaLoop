add_include_dir(.)

add_executable (hulaloop-launcher main.cpp qml.qrc Updater.cpp)

if (WIN32 AND HL_BUILD_GUI)
    find_package (OpenSSL)

    if (${OPENSSL_SSLEAY})
        add_custom_command (
            TARGET hulaloop-launcher
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${OPENSSL_SSLEAY} ${CMAKE_BINARY_DIR}/bin
        )
    endif ()

    if (${OPENSSL_LIBEAY})
        add_custom_command (
            TARGET hulaloop-launcher
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${OPENSSL_LIBEAY} ${CMAKE_BINARY_DIR}/bin
        )
    endif ()

endif ()

target_link_libraries (hulaloop-launcher ${HL_LIBRARIES})