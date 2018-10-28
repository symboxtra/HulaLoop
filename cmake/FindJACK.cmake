# Find JACK
find_library (
    JACK_LIBRARY
    NAMES jack
)

if (NOT JACK_LIBRARY)
    message (FATAL_ERROR "Could not find library: JACK")
else ()
    message (STATUS "JACK Location: ${JACK_LIBRARY}")
endif ()

# Find JACK includes
find_path (
    JACK_PATH
    NAMES "jack/jack.h"
)
if (NOT JACK_PATH)
    message(FATAL_ERROR "Could not find JACK headers.")
else ()
    message (STATUS "JACK Headers: ${JACK_PATH}")
endif ()