# Manually generate and build a CMake project at generation time
function (generate_and_install_lib _path _generation_flags)
    set (PROCESS_RESULT 0)

    # Strip the name from the path
    get_filename_component(_name ${_path} NAME)

    message (STATUS "Building and installing library ${_name} to ${CMAKE_INSTALL_PREFIX}...\n")

    # Manually generate and install
    execute_process (COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/${_name})
    execute_process (COMMAND ${CMAKE_COMMAND} ${_path} ${_generation_flags} -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${_name}
        RESULT_VARIABLE PROCESS_RESULT
    )

    # Make sure generation did not fail
    if (NOT PROCESS_RESULT EQUAL 0)
        message (FATAL_ERROR "\nCMake generation of ${_name} failed with return code ${PROCESS_RESULT}\n")
    endif ()

    execute_process (COMMAND ${CMAKE_COMMAND} --build . --target install
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${_name}
        RESULT_VARIABLE PROCESS_RESULT
    )

    # Make sure build did not fail
    if (NOT PROCESS_RESULT EQUAL 0)
        message (FATAL_ERROR "\nCMake build of ${_name} failed with return code ${PROCESS_RESULT}\n")
    endif ()

    message (STATUS "")
    message (STATUS "Built and installed ${_name}.")
    message (STATUS "Continuing CMake generation...\n")

endfunction ()

function (add_library_target _name _src_files)

    # Generate libraries
    add_library (${_name} STATIC ${_src_files})
    set_target_properties (${_name} PROPERTIES
        VERSION ${VERSION}
        SOVERSION 1
        PUBLIC_HEADER include/${_name}/${_name}.h
    )

    # Target public and private headers
    target_include_directories (${_name} PUBLIC include)
    target_include_directories (${_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

    # Copy the include directory
    install (DIRECTORY include/${_name}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
    )

    # Copy the library
    install (TARGETS ${_name}
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin                               # DLLs
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib                               # Static libs
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib                               # Shared libs (non-DLL)
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${_name}  # Headers
        COMPONENT library
    )

    install (FILES)

endfunction ()

# Macro for collectinrheader files
macro (add_include_dir)
    file (RELATIVE_PATH _relPath "${PROJECT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    foreach (_dir ${ARGN})
        if (_relPath)
            list (APPEND INCLUDE_DIRS "${_relPath}/${_dir}")
        else ()
            list (APPEND INCLUDE_DIRS "${_dir}")
        endif ()
    endforeach()
    if (_relPath)
        # propagate DIRS to parent directory
        set (INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)
    endif ()
endmacro ()