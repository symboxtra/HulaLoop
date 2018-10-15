add_include_dir(.)

add_executable(hulaloop main.cpp qmlbridge.cpp ../control/transport.cpp qml.qrc)

target_link_libraries(hulaloop ${HL_LIBRARIES})

install(TARGETS hulaloop
    RUNTIME
    DESTINATION bin
    COMPONENT app
)

if (WIN32)
	qt5_post_build_windeployqt(hulaloop ${CMAKE_SOURCE_DIR}/src/ui/qml)
endif ()