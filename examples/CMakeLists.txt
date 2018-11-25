######## Config ########

cmake_minimum_required (VERSION 3.5)

set (CMAKE_CXX_STANDARD 14)

# Declare project
project (HulaLoop-Examples LANGUAGES CXX C)

include_directories (../src/audio/include ../src/libs/portaudio/include ../src/libs/portaudio/src/common)
add_executable (hulaloop-simple-example Simple.cpp)

target_link_libraries (hulaloop-simple-example hlaudio)
