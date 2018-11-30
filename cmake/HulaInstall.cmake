set (CPACK_PACKAGE_NAME "HulaLoop Installer")
set (CPACK_PACKAGE_VENDOR "Symboxtra Software")
set (CPACK_PACKAGE_CONTACT "dev.symboxtra@gmail.com")
set (CPACK_PACKAGE_HOMEPAGE_URL "https://hulaloop.readthedocs.io/")

set (CPACK_PACKAGE_VERSION HL_VERSION_STR)
set (CPACK_PACKAGE_FILE_NAME "HulaLoop Installer")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "Simple cross-platform audio loopback and recording.")
set (CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/res/eula.txt)

set (HL_PACKAGE_TYPE ".tar.gz")
if (WIN32 AND 64BIT)

    set (HL_PACKAGE_TYPE "win64.exe")

elseif (WIN32 AND NOT 64BIT)

    set (HL_PACKAGE_TYPE "win32.exe")

elseif (OSX)

    set (HL_PACKAGE_TYPE ".dmg")

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

######## CPack ########
if (64BIT) # Windows x64

    set (CPACK_GENERATOR "NSIS64")

    set (CPACK_NSIS_MUI_ICON "${PROJECT_SOURCE_DIR}/res/logo/hulaloop-logo.ico")
    set (CPACK_NSIS_MUI_UNIICON "${PROJECT_SOURCE_DIR}/res/logo/hulaloop-logo.ico")
    set (CPACK_NSIS_INSTALLED_ICON_NAME "${PROJECT_SOURCE_DIR}/res/logo/hulaloop-logo.ico")
    set (CPACK_PACKAGE_INSTALL_DIRECTORY "Test CPack")
    set (CPACK_NSIS_MODIFY_PATH ON)
    set (CPACK_COMPONENTS_ALL app)

    if (PACK)
        set (INSTALL_DIR bin)
        set (LIB_DIR bin/lib)
    endif ()

elseif (WIN32) # Windows x86
    set (CPACK_GENERATOR "NSIS")

    set (CPACK_NSIS_MUI_ICON "${PROJECT_SOURCE_DIR}/res/logo/hulaloop-logo.ico")
    set (CPACK_NSIS_MUI_UNIICON "${PROJECT_SOURCE_DIR}/res/logo/hulaloop-logo.ico")
    set (CPACK_NSIS_INSTALLED_ICON_NAME "${PROJECT_SOURCE_DIR}/res/logo/hulaloop-logo.ico")
    set (CPACK_PACKAGE_INSTALL_DIRECTORY "Test CPack")
    set (CPACK_NSIS_MODIFY_PATH ON)
    set (CPACK_COMPONENTS_ALL app)

    if (PACK)
        set (INSTALL_DIR bin)
        set (LIB_DIR bin/lib)
    endif ()
endif ()

include (CPack)