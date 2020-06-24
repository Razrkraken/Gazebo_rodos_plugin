cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(rodos_plugin CXX C)

set(CMAKE_CXX_STANDARD 14)


# APPEND Findrodos.cmake address
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Find Rodos
#add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-fPIC)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
include(deps/rodos/cmake/port/posix.cmake)
add_subdirectory(deps/rodos)

# Find Gazebo
find_package(gazebo REQUIRED)

include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS} -Wl,--no-undefined")

# Find threading Library
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads)

# Build our plugins
#Build Rodos_plugin
add_library(rodos_plugin SHARED src/rodos_plugin.cc)
target_link_libraries(rodos_plugin ${GAZEBO_LIBRARIES} rodos Threads::Threads ${CMAKE_DL_LIBS})

###########################################################################

#${CPP_COMP}   ${CFLAGS} ${CPPFLAGS} ${INCLUDES} ${FIRST_LINK_OBJ} -o tst  $* ${LINKFLAGS}

#add_custom_command(TARGET rodos_plugin POST_BUILD COMMAND
#        ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/world/Glider $ENV{HOME}/.gazebo/models/Glider)
#
#add_custom_command(TARGET rodos_plugin POST_BUILD COMMAND
#        ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/world/cessna $ENV{HOME}/.gazebo/models/cessna)
#

#extra information No relevant code
# -D CMAKE_FIND_DEBUG_MODE=ON
#CMAKE_PREFIX_PATH=/home/alejandro/rodos-workspace


add_custom_target(
        test
        COMMAND gazebo --verbose example/world/Glider.world
        DEPENDS rodos_plugin
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

