cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 14)

# Find Gazebo
find_package(gazebo REQUIRED)

include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS} ")

add_library(rodos_plugin SHARED src/rodos_plugin.cc)
target_link_libraries(rodos_plugin ${GAZEBO_LIBRARIES})

