cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 14)

include(deps/rodos/cmake/port/linux-x86.cmake)
add_subdirectory(deps/rodos)

add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
add_executable(rodos_system src/plugin_connector.cpp)
target_link_libraries(rodos_system PUBLIC rodos)
