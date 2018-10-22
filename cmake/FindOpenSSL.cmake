#set (OPENSSL_LIBRARY "")
set (OPENSSL_PATH64 "C:\\OpenSSL-Win64\\bin")
set (OPENSSL_PATH32 "C:\\OpenSSL-Win32\\bin")

if (WIN32 AND 64BIT)

    set (CMAKE_FIND_LIBRARY_PREFIXES "")
    set (CMAKE_FIND_LIBRARY_SUFFIXES ".dll")

    find_library (
        OPENSSL_LIBRARY
        NAMES ssleay32 libeay32
        PATHS ${OPENSSL_PATH64}
        NO_DEFAULT_PATH
    )
elseif (WIN32)
    find_library (
        OPENSSL_LIBRARY
        NAMES libeay32 ssleay32
        PATHS ${OPENSSL_PATH32}
    )
endif ()

if (NOT OPENSSL_LIBRARY)
    message (FATAL_ERROR "Could not find library: OpenSSL")
else ()
    message (STATUS "OpenSSL Location: ${OPENSSL_LIBRARY}")
endif ()