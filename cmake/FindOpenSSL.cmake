set (OPENSSL_PATH64 "C:\\OpenSSL-Win64\\bin")
set (OPENSSL_PATH32 "C:\\OpenSSL-Win32\\bin")

if (WIN32 AND 64BIT)

    set (CMAKE_FIND_LIBRARY_PREFIXES "")
    set (CMAKE_FIND_LIBRARY_SUFFIXES ".dll")

    find_library (
        OPENSSL_SSLEAY
        NAMES ssleay32
        PATHS ${OPENSSL_PATH64}
        NO_DEFAULT_PATH
    )
    find_library (
        OPENSSL_LIBEAY
        NAMES libeay32
        PATHS ${OPENSSL_PATH64}
        NO_DEFAULT_PATH
    )

elseif (WIN32)

    find_library (
        OPENSSL_SSLEAY
        NAMES ssleay32
        PATHS ${OPENSSL_PATH32}
        NO_DEFAULT_PATH
    )
    find_library (
        OPENSSL_LIBEAY
        NAMES libeay32
        PATHS ${OPENSSL_PATH64}
        NO_DEFAULT_PATH
    )

endif ()

if (NOT OPENSSL_SSLEAY)
    message (WARNING "Could not find library: OpenSSL (ssleay32.dll)")
else ()
    message (STATUS "OpenSSL ssleay32.dll location: ${OPENSSL_SSLEAY}")
endif ()

if (NOT OPENSSL_LIBEAY)
    message (WARNING "Could not find library: OpenSSL (libeay32.dll)")
else ()
    message (STATUS "OpenSSL libeay32.dll location: ${OPENSSL_LIBEAY}")
endif ()