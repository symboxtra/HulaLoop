add_executable (hulaloop-launcher main.cpp qml.qrc Updater.cpp)

find_package (OpenSSL REQUIRED)

add_custom_command (
    TARGET hulaloop-launcher
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${OPENSSL_SSLEAY} ${CMAKE_BINARY_DIR}/bin
)

add_custom_command (
    TARGET hulaloop-launcher
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${OPENSSL_LIBEAY} ${CMAKE_BINARY_DIR}/bin
)

target_link_libraries (hulaloop-launcher ${HL_LIBRARIES})