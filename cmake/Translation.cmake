# Linguist tools for translation
# This is optional
# The base Qt5 package is found based on the Qt5_DIR set in the root CMakeLists.txt
find_package (Qt5
    OPTIONAL_COMPONENTS LinguistTools
)
set (FOUND_QT_LINGUIST FALSE)
if (Qt5_LUPDATE_EXECUTABLE AND Qt5_LRELEASE_EXECUTABLE)
    message (STATUS "Qt5LinguistTools: Found")
    set (FOUND_QT_LINGUIST TRUE)
endif ()

# Generate translations
# Based on code from https://gist.github.com/02JanDal/70a39f2cc3d2002b3588
if (FOUND_QT_LINGUIST)

    # Glob all the translation files
    # To add a language, create an empty ts file named hulaloop_XX.ts in the res/translations folder
    # See the ISO language codes for the XX (https://www.andiamo.co.uk/resources/iso-language-codes)
    # This glob will pick it up and fill it with the English entries for translation
    file (GLOB TRANSLATIONS_FILES ${PROJECT_SOURCE_DIR}/res/translations/*.ts)
    set_directory_properties (PROPERTIES CLEAN_NO_CUSTOM TRUE)

    # Update the .ts files with strings from the application source
    qt5_create_translation (QM_FILES
        ${PROJECT_SOURCE_DIR}/src/control ${PROJECT_SOURCE_DIR}/src/launcher ${PROJECT_SOURCE_DIR}/src/ui
        ${TRANSLATIONS_FILES}
        OPTIONS -recursive
    )

    # Convert the XML-based .ts files to binary .qm files
    qt5_add_translation (TRANSLATION_QM ${TRANSLATIONS_FILES})

    # Add the generated commands as targets
    add_custom_target (TranslationUpdate ALL DEPENDS ${QM_FILES})
    add_custom_target (Translation ALL DEPENDS ${TRANSLATION_QM})
    add_dependencies (Translation TranslationUpdate)

    # Add the targets as dependencies to the main application
    if (HL_BUILD_GUI)
        add_dependencies (hulaloop TranslationUpdate)
        add_dependencies (hulaloop-launcher TranslationUpdate)
    endif ()

    if (HL_BUILD_CLI)
        add_dependencies (hulaloop-cli TranslationUpdate)
    endif ()

    # Install information for the translation files
    if (OSX)
        # This should eventually be the OSX bundle
        install (FILES ${TRANSLATION_QM} DESTINATION bin)
    else ()
        install (FILES ${TRANSLATION_QM} DESTINATION bin)
    endif ()
endif ()