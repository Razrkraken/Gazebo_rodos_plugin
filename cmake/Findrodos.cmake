# Look for the header file.
set(RODOS_ROOT /home/alejandro/GAZEBO_SIMS/rodos_plugin/RODOS/rodos)  #TODO: Relative path
#
set(RODOS_INCLUDE_DIR ${RODOS_ROOT}/api ${RODOS_ROOT}/src/independent ${RODOS_ROOT}/src/independent/gateway
        ${RODOS_ROOT}/src/bare-metal-generic ${RODOS_ROOT}/src/bare-metal/linux_x86 ${RODOS_ROOT}/../support_libs)
#
## Look for the library.
find_library(RODOS_LIBRARY NAMES librodos.a PATHS ${RODOS_ROOT}/build-linux/)
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Rodos DEFAULT_MSG RODOS_LIBRARY RODOS_INCLUDE_DIR)

#set(ARCH linux_x86)
#set(MY_SRCS ${RODOS_ROOT}/src/bare-metal-generic)
#set(MY_HW_SRCS ${RODOS_ROOT}/src/bare-metal/${ARCH})
#set(RODOS_SUPPORT_LIBS /home/alejandro/rodos-workspace/support_libs)

#add_subdirectory(${RODOS_ROOT} ${CMAKE_BINARY_DIR}/rodos)
#set(RODOS_LIBRARIES rodos)

# Copy the results to the output variables.
if(RODOS_FOUND)
    set(RODOS_LIBRARIES ${RODOS_LIBRARY})
    set(RODOS_INCLUDE_DIRS ${RODOS_INCLUDE_DIR})
else()
    set(RODOS_LIBRARIES)
    set(RODOS_INCLUDE_DIRS)
endif()

mark_as_advanced(RODOS_INCLUDE_DIRS RODOS_LIBRARIES)