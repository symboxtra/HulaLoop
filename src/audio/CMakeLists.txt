add_include_dir (.)

file (GLOB AUDIO_SRC_FILES *.cpp)

if(NOT WIN32)
    list(REMOVE_ITEM AUDIO_SRC_FILES "/mnt/c/Users/patel/Documents/GitHub/SplitSound/HulaLoop/src/audio/WindowsAudio.cpp")
endif()



include_directories (Device.h)
set (TEST_FILE Device.cpp)
#MESSAGE (STATUS ${AUDIO_SRC_FILES})
add_executable(test_windows ${AUDIO_SRC_FILES})
if(WIN32)
    target_link_libraries(test_windows winmm Ole32)
else()
    target_link_libraries(test_windows)

endif()
