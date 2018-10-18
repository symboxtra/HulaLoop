add_include_dir(.)

add_executable(hulaloop main.cpp qmlbridge.cpp ../control/transport.cpp ../control/Record.cpp qml.qrc)

target_link_libraries(hulaloop ${HL_LIBRARIES})

if (WIN32)
	qt5_post_build_windeployqt(hulaloop ${CMAKE_SOURCE_DIR}/src/ui/qml)
endif ()