# Look for the header file.

find_path(RODOS_INCLUDE_DIR NAMES rodos.h)


# Look for the library.
find_library(RODOS_LIBRARY NAMES rodos)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(RODOS DEFAULT_MSG RODOS_LIBRARY RODOS_INCLUDE_DIR)

# Copy the results to the output variables.
if(RODOS_FOUND)
    set(RODOS_LIBRARIES ${RODOS_LIBRARY})
    set(RODOS_INCLUDE_DIRS ${RODOS_INCLUDE_DIR})
else()
    set(RODOS_LIBRARIES)
    set(RODOS_INCLUDE_DIRS)
endif()

mark_as_advanced(RODOS_INCLUDE_DIRS RODOS_LIBRARIES)
