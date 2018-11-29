add_include_dir(.)

set (GUI_SRC_FILES main.cpp QMLBridge.cpp SystemTrayIcon.cpp qml.qrc)

# Turn the console off if this is a non-debug build
if (CMAKE_BUILD_TYPE MATCHES Debug)
	add_executable(hulaloop ${GUI_SRC_FILES})
else ()
	add_executable(hulaloop WIN32 ${GUI_SRC_FILES})
endif ()

target_link_libraries(hulaloop ${HL_LIBRARIES})

if (WIN32)
	qt5_post_build_windeployqt(hulaloop ${PROJECT_SOURCE_DIR}/src/ui/gui/qml)
endif ()