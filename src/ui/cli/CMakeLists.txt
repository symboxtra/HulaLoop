add_include_dir (.)

add_executable (hulaloop-cli main.cpp HulaInteractiveCli.cpp)
target_link_libraries(hulaloop-cli ${HL_LIBRARIES})
