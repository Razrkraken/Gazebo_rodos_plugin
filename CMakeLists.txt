cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)


project(rodos_plugin CXX C)
set(CMAKE_CXX_STANDARD 14)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(rodos REQUIRED)
# Find Gazebo
find_package(gazebo REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS}")

# Build our plugin
add_library(rodos_plugin SHARED rodos_plugin.cc)
target_link_libraries(rodos_plugin ${GAZEBO_LIBRARIES})

add_custom_command(TARGET rodos_plugin POST_BUILD COMMAND
        ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/world/Glider $ENV{HOME}/.gazebo/models/Glider)


# Build the stand-alone test program
# add_executable(vel vel.cc)

#if (${gazebo_VERSION_MAJOR} LESS 6)
#  # These two
#  include(FindBoost)
#  find_package(Boost ${MIN_BOOST_VERSION} REQUIRED system filesystem regex)
#  target_link_libraries(vel ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES})
#else()
#  target_link_libraries(vel ${GAZEBO_LIBRARIES})
#endif()
# -D CMAKE_FIND_DEBUG_MODE=ON
#CMAKE_PREFIX_PATH=/home/alejandro/rodos-workspace