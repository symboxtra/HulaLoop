add_include_dir(.)

add_executable(hulaloop main.cpp QMLBridge.cpp SystemTrayIcon.cpp qml.qrc)
target_link_libraries(hulaloop ${HL_LIBRARIES})

if (WIN32)
	qt5_post_build_windeployqt(hulaloop ${PROJECT_SOURCE_DIR}/src/ui/gui/qml)
endif ()