add_include_dir(.)

add_executable(hulaloop main.cpp qmlbridge.cpp ../control/transport.cpp qml.qrc ${AUDIO_SRC_FILES})

target_link_libraries(hulaloop ${HL_LIBRARIES})

if (WIN32)
	qt5_post_build_windeployqt(hulaloop ${CMAKE_SOURCE_DIR}/src/ui/qml)
endif ()