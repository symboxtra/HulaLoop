set (HL_LIBRARY_NAME "hlcontrol")

file (GLOB CONTROL_SRC_FILES *.cpp)

# Add library
add_library_target (${HL_LIBRARY_NAME} "${CONTROL_SRC_FILES}")
add_include_dir (${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link external libraries to new generated library
target_link_libraries (${HL_LIBRARY_NAME} ${HL_LIBRARIES})

set (HL_LIBRARIES ${HL_LIBRARIES} ${HL_LIBRARY_NAME} PARENT_SCOPE)
