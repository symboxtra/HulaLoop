set (HL_NODE_TARGET "hulaloop-node")

# Make sure we're building from cmake-js
if (CMAKE_JS_VERSION)
    include_directories (${CMAKE_JS_INC})

    file (GLOB ADDON_SOURCE_FILES *.cpp)
    add_library (${HL_NODE_TARGET} SHARED ${ADDON_SOURCE_FILES} ${CMAKE_JS_SRC})
    set_target_properties (${HL_NODE_TARGET} PROPERTIES PREFIX "" SUFFIX ".node")
    target_link_libraries (${HL_NODE_TARGET} ${HL_LIBRARIES} ${CMAKE_JS_LIB})

    # Find and include StreaminWorker header
    message (STATUS "Finding streaming-worker-sdk module...")
    execute_process (
        COMMAND node -p "require('streaming-worker-sdk').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE STREAMING_WORKER_INCLUDE_DIR
    )
    string (LENGTH "${STREAMING_WORKER_INCLUDE_DIR}" DIR_LENGTH)

    if (DIR_LENGTH)
        # Strip off unwanted extras
        string (REPLACE "\"" "" STREAMING_WORKER_INCLUDE_DIR ${STREAMING_WORKER_INCLUDE_DIR})
        string (REPLACE "undefined" "" STREAMING_WORKER_INCLUDE_DIR ${STREAMING_WORKER_INCLUDE_DIR})
        string (REPLACE "\n" "" STREAMING_WORKER_INCLUDE_DIR ${STREAMING_WORKER_INCLUDE_DIR})

        target_include_directories (${HL_NODE_TARGET} PRIVATE ${PROJECT_SOURCE_DIR}/${STREAMING_WORKER_INCLUDE_DIR})
        message (STATUS "Found streaming-worker-sdk at: ${STREAMING_WORKER_INCLUDE_DIR}")
    else ()
        message (STATUS "Could not find streaming-worker-sdk.")
        message (STATUS "")

        message (FATAL_ERROR "Could not find streaming-worker-sdk.")
    endif ()
else ()
    message (STATUS "The ${HL_NODE_TARGET} addon can only be built from cmake-js.")
    message (STATUS "Try running npm install or cmake-js build")
    message (STATUS "")

    message (FATAL_ERROR "Build not running from cmake-js.")
endif ()