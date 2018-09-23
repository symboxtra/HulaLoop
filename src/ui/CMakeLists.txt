add_executable(hulaloop main.cpp qml.qrc)

target_link_libraries(hulaloop ${HL_LIBRARIES})

if (WIN32)
    qt5_post_build_windeployqt(hulaloop ${CMAKE_SOURCE_DIR}/src/ui/qml)
endif ()