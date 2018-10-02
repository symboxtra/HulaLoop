add_include_dir (.)

file (GLOB AUDIO_SRC_FILES *.cpp)

# Remove source files that do not pertain to the current OS
#TODO: Remove extra source files once they have been initialized
if(WIN32)
    list(REMOVE_ITEM AUDIO_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/LinuxAudio.cpp")
elseif(OSX)
    list(REMOVE_ITEM AUDIO_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/WindowsAudio.cpp")
else()
    list(REMOVE_ITEM AUDIO_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/WindowsAudio.cpp")
endif()

#TODO: Change this to better apply with the parent CMake
add_executable(test_windows ${AUDIO_SRC_FILES})
if(WIN32)
    target_link_libraries(test_windows winmm Ole32)
elseif(OSX)
else()
    target_link_libraries(test_windows asound pthread)
endif()

## Generate shared libraries

add_library (hul SHARED
    Controller.cpp
    Device.cpp
    OSAudio.cpp
    WindowsAudio.cpp
)

set_target_properties(hul PROPERTIES
    VERSION ${VERSION}
    SOVERSION 1
    PUBLIC_HEADER Controller.h
)

set(CMAKE_INSTALL_LIBDIR ${CMAKE_PROJECT})
target_include_directories(hul PRIVATE .)
install(TARGETS hul
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/includes
    COMPONENT library
)

if(WIN32)
    target_link_libraries(hul winmm Ole32)
elseif(OSX)
else()
    target_link_libraries(test_windows asound pthread)
endif()