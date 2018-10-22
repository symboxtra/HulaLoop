add_executable (hulaloop-launcher main.cpp qml.qrc Updater.cpp)

#find_package (OpenSSL REQUIRED)

target_link_libraries (hulaloop-launcher ${HL_LIBRARIES})# ${OPENSSL_LIBRARY})