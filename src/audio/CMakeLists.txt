add_include_dir (.)

file (GLOB AUDIO_SRC_FILES *.cpp)
include_directories (Device.h)
set (TEST_FILE Device.cpp)
#MESSAGE (STATUS ${AUDIO_SRC_FILES})
add_executable(test_windows ${AUDIO_SRC_FILES})
target_link_libraries(test_windows winmm)
