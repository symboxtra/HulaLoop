# Macro to call windeployqt after building
macro (qt5_post_build_windeployqt APP QMLDIR)

    # Get bin directory of Qt from Qt5Core.dll
    find_package (Qt5 REQUIRED Core)
    get_target_property (Qt5_CoreLocation Qt5::Core LOCATION)
    get_filename_component (QTBIN ${Qt5_CoreLocation} DIRECTORY)

    find_program(WINDEPLOYQT_EXECUTABLE
        NAMES
            "windeployqt"
        PATHS
            "${QTBIN}"

        NO_SYSTEM_ENVIRONMENT_PATH
    )

    message (STATUS "WINDEPLOYQT: ${WINDEPLOYQT_EXECUTABLE}")

    if (${WINDEPLOYQT_EXECUTABLE} STREQUAL "WINDEPLOYQT_EXECUTABLE-NOTFOUND")
        message (FATAL_ERROR "\nCould not find windeployqt.\n")
    endif ()

    add_custom_command (TARGET ${APP} POST_BUILD
        COMMAND ${WINDEPLOYQT_EXECUTABLE} --verbose 0 -qmldir ${QMLDIR} $<TARGET_FILE:${APP}>
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )

    message (STATUS "Added post-build call to windeployqt.")

endmacro ()

# Macro to call macdeployqt after building
macro (qt5_post_build_macdeployqt APP QMLDIR)

    find_program(MACDEPLOYQT_EXECUTABLE
        NAMES "macdeployqt"
    )

    if (${MACDEPLOYQT_EXECUTABLE} STREQUAL "MACDEPLOYQT_EXECUTABLE-NOTFOUND")
        message (FATAL_ERROR "\nCould not find macdeployqt. Please make sure the Qt bin folder is on PATH.\n")
    endif ()

    add_custom_command (TARGET ${APP} POST_BUILD
        COMMAND ${MACDEPLOYQT_EXECUTABLE} -qmldir ${QMLDIR} $<TARGET_FILE:${APP}>
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )

    message (STATUS "Added post-build call to macdeployqt.")

endmacro ()