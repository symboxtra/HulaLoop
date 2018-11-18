# Setup for libsndfile
if (WIN32 AND NOT DEFINED LIBSND_ROOT)
    if (DEFINED ENV{LIBSND_ROOT})
        set (LIBSND_ROOT $ENV{LIBSND_ROOT} CACHE STRING "Path to Sndfile Installation")
    else ()
        message (FATAL_ERROR "LIBSND_ROOT environment variable not set. Add the environment variable in Windows Environment Variables or cmake with -DLIBSND_ROOT=C:\\path\\to\\libsnd\\lib")
    endif ()
elseif (WIN32)
    set (LIBSND_ROOT $ENV{LIBSND_ROOT} CACHE STRING "Path to Sndfile Installation")
endif ()

message (STATUS "LIBSND_ROOT: " ${LIBSND_ROOT} "\n")

# Required Sndfile libraries and includes
set (LIBSND_LIB_PATH ${LIBSND_ROOT}/lib)
set (LIBSND_INCLUDE_PATH ${LIBSND_ROOT}/include)
set (LIBSND_BIN_PATH ${LIBSND_ROOT}/bin)

find_library (LIBSNDFILE_LIB
        NAMES sndfile libsndfile-1
        HINTS ${LIBSND_LIB_PATH})

find_path (LIBSNDFILE_INCLUDE
        NAMES sndfile.h
        HINTS ${LIBSND_INCLUDE_PATH})

set (CMAKE_FIND_LIBRARY_PREFIXES "")
set (CMAKE_FIND_LIBRARY_SUFFIXES ".dll")

find_library (LIBSNDFILE_DLL
        NAMES libsndfile-1
        HINTS ${LISSND_BIN_PATH})


MESSAGE (STATUS "LibSndFile Location: ${LIBSNDFILE_DLL}")

# Fail CMake based on passed find_package arguments (if SndFile is not found)
find_package (PackageHandleStandardArgs)
find_package_handle_standard_args (SndFile DEFAULT_MSG LIBSNDFILE_LIB LIBSNDFILE_INCLUDE)