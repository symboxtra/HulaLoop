set (CPACK_PACKAGE_NAME "HulaLoop")
set (CPACK_PACKAGE_VENDOR "Symboxtra Software")
set (CPACK_PACKAGE_CONTACT "dev.symboxtra@gmail.com")
set (CPACK_PACKAGE_HOMEPAGE_URL "https://hulaloop.readthedocs.io/")

set (CPACK_PACKAGE_VERSION ${HL_VERSION_STR})
set (CPACK_PACKAGE_FILE_NAME "hulaloop-setup-${HL_VERSION_STR}")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "Simple cross-platform audio loopback and recording.")
set (CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/res/eula.txt)

set (HL_PACKAGE_TYPE ".tar.gz")
if (WIN32 OR OSX)

    set (CPACK_GENERATOR "IFW")
    set (CPACK_IFW_VERBOSE ON)

    if (WIN32 AND 64BIT)
        set (HL_PACKAGE_TYPE "win64.exe")
        set (CPACK_IFW_PACKAGE_ICON ${PROJECT_SOURCE_DIR}/res/logo/hulaloop-logo.ico)
        set (CPACK_PACKAGE_FILE_NAME "hulaloop-setup-${HL_VERSION_STR}-win64")
    elseif (WIN32 AND NOT 64BIT)
        set (HL_PACKAGE_TYPE "win32.exe")
        set (CPACK_IFW_PACKAGE_ICON ${PROJECT_SOURCE_DIR}/res/logo/hulaloop-logo.ico)
        set (CPACK_PACKAGE_FILE_NAME "hulaloop-setup-${HL_VERSION_STR}-win32")
    elseif (OSX)
        set (HL_PACKAGE_TYPE ".dmg")
        set (CPACK_IFW_PACKAGE_ICON ${PROJECT_SOURCE_DIR}/res/logo/hulaloop-logo.icns)
    endif ()

    set (CPACK_IFW_PACKAGE_TITLE "HulaLoop")
    set (CPACK_IFW_PACKAGE_WIZARD_STYLE "Modern")
    set (CPACK_IFW_PACKAGE_WINDOW_ICON ${PROJECT_SOURCE_DIR}/res/logo/hulaloop-logo.png)
    set (CPACK_IFW_PACKAGE_WATERMARK ${PROJECT_SOURCE_DIR}/res/logo/hulaloop-header-small.png)
    set (CPACK_IFW_PACKAGE_TITLE_COLOR "#f75c26")

    set (CPACK_IFW_PACKAGE_START_MENU_DIRECTORY "HulaLoop")
    set (CPACK_IFW_TARGET_DIRECTORY "@ApplicationsDir@/HulaLoop")
    set (CPACK_IFW_ADMIN_TARGET_DIRECTORY "@ApplicationsDir@/HulaLoop")

    set (CPACK_IFW_PACKAGE_CONTROL_SCRIPT ${PROJECT_SOURCE_DIR}/res/installer/ifwscript.qs)
    set (CPACK_IFW_PACKAGE_MAINTENANCE_TOOL_NAME "hulaloop-uninstaller")
    set (CPACK_IFW_PACKAGE_MAINTENANCE_TOOL_INI_FILE "hulaloop-uninstaller")

    set (CPACK_IFW_PACKAGE_ALLOW_NON_ASCII_CHARACTERS OFF)

elseif (EXISTS "/etc/debian_version")

    set (HL_PACKAGE_TYPE ".deb")
    set (CPACK_GENERATOR "DEB")

    set (CPACK_DEBIAN_PACKAGE_DEPENDS "g++, build-essential, cmake, libgl1-mesa-dev, libasound2, libasound2-dev, pavucontrol, qt5-default, libqt5charts5, libqt5quickcontrols2-5")

    # Install logo file and the desktop shortcut
    configure_file (
        "${PROJECT_SOURCE_DIR}/res/installer/hulaloop.desktop.in"
        "${CMAKE_BINARY_DIR}/hulaloop.desktop"
    )
    install (FILES ${CMAKE_BINARY_DIR}/hulaloop.desktop DESTINATION /usr/share/applications)
    install (FILES ${PROJECT_SOURCE_DIR}/res/logo/hulaloop-logo.svg DESTINATION /usr/share/icons/hicolor/scalable/apps)

elseif (EXISTS "/etc/redhat-release")

    set (HL_PACKAGE_TYPE ".rpm")
    set (CPACK_GENERATOR "RPM")

    set (CPACK_RPM_PACKAGE_REQUIRES "gcc-c++, make, cmake, qt5-devel, qt5-qtcharts, qt5-qtquickcontrols2-devel, alsa-lib, alsa-lib-devel")

    # Install logo file and the desktop shortcut
    configure_file (
        "${PROJECT_SOURCE_DIR}/res/installer/hulaloop.desktop.in"
        "${CMAKE_BINARY_DIR}/hulaloop.desktop"
    )
    install (FILES ${CMAKE_BINARY_DIR}/hulaloop.desktop DESTINATION /usr/share/applications)
    install (FILES ${PROJECT_SOURCE_DIR}/res/logo/hulaloop-logo.svg DESTINATION /usr/share/icons/hicolor/scalable/apps)

endif ()

include (CPack)