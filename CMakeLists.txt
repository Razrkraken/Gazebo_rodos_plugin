cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(rodos_plugin CXX C)
add_subdirectory(gazebo_connector)
add_subdirectory(rodos_connector)

add_custom_target(
        test
        COMMAND gazebo --verbose example/world/Glider.world
        DEPENDS rodos_system rodos_plugin
)